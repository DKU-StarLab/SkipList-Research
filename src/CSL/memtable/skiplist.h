//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Thread safety
// -------------
//
// Writes require external synchronization, most likely a mutex.
// Reads require a guarantee that the SkipList will not be destroyed
// while the read is in progress.  Apart from that, reads progress
// without any internal locking or synchronization.
//
// Invariants:
//
// (1) Allocated nodes are never deleted until the SkipList is
// destroyed.  This is trivially guaranteed by the code since we
// never delete any skip list nodes.
//
// (2) The contents of a Node except for the next/prev pointers are
// immutable after the Node has been linked into the SkipList.
// Only Insert() modifies the list, and it is careful to initialize
// a node and use release-stores to publish the nodes in one or
// more lists.
//
// ... prev vs. next pointer ordering ...
//

#pragma once
#include <assert.h>
#include <stdlib.h>
#include <atomic>
#include "memory/allocator.h"
#include "port/port.h"
#include "util/random.h"

//#define MOD 32 // Signal.Jin
#define ARR_SIZE 4 // Signal.Jin

namespace ROCKSDB_NAMESPACE {

template<typename Key, class Comparator>
class SkipList {
 private:
  struct Node;

 public:
  // Create a new SkipList object that will use "cmp" for comparing keys,
  // and will allocate memory using "*allocator".  Objects allocated in the
  // allocator must remain allocated for the lifetime of the skiplist object.
  explicit SkipList(Comparator cmp, Allocator* allocator,
                    int32_t max_height = 12, int32_t branching_factor = 4);
  // No copying allowed
  SkipList(const SkipList&) = delete;
  void operator=(const SkipList&) = delete;

  // Insert key into the list.
  // REQUIRES: nothing that compares equal to key is currently in the list.
  void Insert(const Key& key);

  void Insert_Buf(const Key& key); // Signal.Jin

  unsigned int log2_for_power_of_2(unsigned int val);

 // Function to determine maximum  height - zhao
  int GetMaxHeight_test() const { return kMaxHeight_; } 
  int GetHeight_now() const { return max_height_.load(std::memory_order_relaxed); }

  uint64_t Estimate_Max() const; // Signal.Jin

  // Returns true iff an entry that compares equal to key is in the list.
  bool Contains(const Key& key) const;


  // Signal.Jin - Scan operation
  bool Contains_OriScan(const Key& key, uint64_t scan_num) const;
  bool Contains_Scan(const Key& key, uint64_t scan_num) const;

  // Signal.Jin
  bool Contains_Buf(const Key& key) const;

  // Return estimated number of entries smaller than `key`.
  uint64_t EstimateCount(const Key& key) const;

  uint64_t EstimateNode() const; // Signal.Jin

  // Iteration over the contents of a skip list
  class Iterator {
   public:
    // Initialize an iterator over the specified list.
    // The returned iterator is not valid.
    explicit Iterator(const SkipList* list);

    // Change the underlying skiplist used for this iterator
    // This enables us not changing the iterator without deallocating
    // an old one and then allocating a new one
    void SetList(const SkipList* list);

    // Returns true iff the iterator is positioned at a valid node.
    bool Valid() const;

    // Returns the key at the current position.
    // REQUIRES: Valid()
    const Key& key() const;

    // Advances to the next position.
    // REQUIRES: Valid()
    void Next();

    // Advances to the previous position.
    // REQUIRES: Valid()
    void Prev();

    // Advance to the first entry with a key >= target
    void Seek(const Key& target);

    // Retreat to the last entry with a key <= target
    void SeekForPrev(const Key& target);

    // Position at the first entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToFirst();

    // Position at the last entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToLast();

   private:
    const SkipList* list_;
    Node* node_;
    // Intentionally copyable
  };

 private:

  // Delete "const" for dynamic height adjustment  - zhao
  uint16_t kMaxHeight_;
  const uint16_t kBranching_;
  const uint32_t kScaledInverseBranching_;

  // Immutable after construction
  Comparator const compare_;
  Allocator* const allocator_;    // Allocator used for allocations of nodes

  Node* const head_;
  
  unsigned int MOD;
  mutable unsigned int log_mod_; // Signal.Jin

  // Modified only by Insert().  Read racily by readers, but stale
  // values are ok.
  std::atomic<int> max_height_;  // Height of the entire list

  // Used for optimizing sequential insert patterns.  Tricky.  prev_[i] for
  // i up to max_height_ is the predecessor of prev_[0] and prev_height_
  // is the height of prev_[0].  prev_[0] can only be equal to head before
  // insertion, in which case max_height_ and prev_height_ are 1.
  Node** prev_;
  int32_t prev_height_;

  inline int GetMaxHeight() const {
    return max_height_.load(std::memory_order_relaxed);
  }

  Node* NewNode(const Key& key, int height);
  Node* NewNode_plus(const Key& key, int height);
  int RandomHeight();
  bool Equal(const Key& a, const Key& b) const { return (compare_(a, b) == 0); }
  bool LessThan(const Key& a, const Key& b) const {
    return (compare_(a, b) < 0);
  }

  // Return true if key is greater than the data stored in "n"
  bool KeyIsAfterNode(const Key& key, Node* n) const;
  
  bool KeyIsAfterNode_plus(const Key& key, Node* n, int level) const;

  // Returns the earliest node with a key >= key.
  // Return nullptr if there is no such node.
  Node* FindGreaterOrEqual(const Key& key) const;

  // Signal.Jin - Scan operation
  Node* FindGreaterOrEqual_OriScan(const Key& key, uint64_t scan_num) const;
  Node* FindGreaterOrEqual_Scan(const Key& key, uint64_t scan_num) const;

  Node* FindGreaterOrEqual_Buf(const Key& key) const;
  Node* FindLessOrEqual_Buf(const Key& key, Node** prev = nullptr) const;

  // Return the latest node with a key < key.
  // Return head_ if there is no such node.
  // Fills prev[level] with pointer to previous node at "level" for every
  // level in [0..max_height_-1], if prev is non-null.
  Node* FindLessThan(const Key& key, Node** prev = nullptr) const;

  // Signal.Jin
  Node* FindLessThan_plus(const Key& key, Node** prev = nullptr) const;

  // Return the last node in the list.
  // Return head_ if list is empty.
  Node* FindLast() const;
};

// Implementation details follow
template<typename Key, class Comparator>
struct SkipList<Key, Comparator>::Node {
  explicit Node(const Key& k) : key(k) { }

  Key const key;

  // Signal.Jin - Buf
  int array_size;
  Key *ptr;

  // Accessors/mutators for links.  Wrapped in methods so we can
  // add the appropriate barriers as necessary.
  Node* Next(int n) {
    assert(n >= 0);
    // Use an 'acquire load' so that we observe a fully initialized
    // version of the returned Node.
    return (next_[n].load(std::memory_order_acquire));
  }
  void SetNext(int n, Node* x) {
    assert(n >= 0);
    // Use a 'release store' so that anybody who reads through this
    // pointer observes a fully initialized version of the inserted node.
    next_[n].store(x, std::memory_order_release);
  }

  Node* Next_plus(int n, int idx) {
    assert(n >= 0);
    return (next_plus[n][idx].load(std::memory_order_acquire));
  }
  void SetNext_plus(int n, Node* x, int idx) {
    assert(n >= 0);
    next_plus[n][idx].store(x, std::memory_order_release);
  }

  // No-barrier variants that can be safely used in a few locations.
  Node* NoBarrier_Next(int n) {
    assert(n >= 0);
    return next_[n].load(std::memory_order_relaxed);
  }
  void NoBarrier_SetNext(int n, Node* x) {
    assert(n >= 0);
    next_[n].store(x, std::memory_order_relaxed);
  }

  Node* NoBarrier_Next_plus(int n, int idx) {
    assert(n >= 0);
    return next_plus[n][idx].load(std::memory_order_relaxed);
  }
  void NoBarrier_SetNext_plus(int n, Node* x, int idx) {
    assert(n >= 0);
    next_plus[n][idx].store(x, std::memory_order_relaxed);
  }

 private:
  // Array of length equal to the node height.  next_[0] is lowest level link.
  std::atomic<Node*> next_[0];
};

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node*
SkipList<Key, Comparator>::NewNode(const Key& key, int height) {
  char* mem = allocator_->AllocateAligned(
      sizeof(Node) + sizeof(std::atomic<Node*>) * (height - 1));
  return new (mem) Node(key);
}

template<typename Key, class Comparator>
inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList* list) {
  SetList(list);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SetList(const SkipList* list) {
  list_ = list;
  node_ = nullptr;
}

template<typename Key, class Comparator>
inline bool SkipList<Key, Comparator>::Iterator::Valid() const {
  return node_ != nullptr;
}

template<typename Key, class Comparator>
inline const Key& SkipList<Key, Comparator>::Iterator::key() const {
  assert(Valid());
  return node_->key;
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Next() {
  assert(Valid());
  node_ = node_->Next(0);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Prev() {
  // Instead of using explicit "prev" links, we just search for the
  // last node that falls before key.
  assert(Valid());
  node_ = list_->FindLessThan(node_->key);
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Seek(const Key& target) {
  node_ = list_->FindGreaterOrEqual(target);
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekForPrev(
    const Key& target) {
  Seek(target);
  if (!Valid()) {
    SeekToLast();
  }
  while (Valid() && list_->LessThan(target, key())) {
    Prev();
  }
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToFirst() {
  node_ = list_->head_->Next(0);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToLast() {
  node_ = list_->FindLast();
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}

template<typename Key, class Comparator>
int SkipList<Key, Comparator>::RandomHeight() {
  auto rnd = Random::GetTLSInstance();

  // Increase height with probability 1 in kBranching
  int height = 1;
  while (height < kMaxHeight_ && rnd->Next() < kScaledInverseBranching_) {
    height++;
  }
  assert(height > 0);
  assert(height <= kMaxHeight_);
  return height;
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::KeyIsAfterNode(const Key& key, Node* n) const {
  // nullptr n is considered infinite
  return (n != nullptr) && (compare_(n->key, key) < 0);
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::
  FindGreaterOrEqual(const Key& key) const {
  // Note: It looks like we could reduce duplication by implementing
  // this function as FindLessThan(key)->Next(0), but we wouldn't be able
  // to exit early on equality and the result wouldn't even be correct.
  // A concurrent insert might occur after FindLessThan(key) but before
  // we get a chance to call Next(0).
  
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  Node* last_bigger = nullptr;
  while (true) {
    assert(x != nullptr);
    Node* next = x->Next(level);
    /*if (next != nullptr) {
      PREFETCH(next->Next(level), 0, 1);
    }*/
    // Make sure the lists are sorted
    assert(x == head_ || next == nullptr || KeyIsAfterNode(next->key, x));
    // Make sure we haven't overshot during our search
    assert(x == head_ || KeyIsAfterNode(key, x));
    int cmp = (next == nullptr || next == last_bigger)
        ? 1 : compare_(next->key, key);
    if (cmp == 0 || (cmp > 0 && level == 0)) {
      return next;
    } else if (cmp < 0) {
      // Keep searching in this list
      x = next;
    } else {
      // Switch to next list, reuse compare_() result
      last_bigger = next;
      level--;
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::
  FindGreaterOrEqual_OriScan(const Key& key, uint64_t scan_num) const {
  // Note: It looks like we could reduce duplication by implementing
  // this function as FindLessThan(key)->Next(0), but we wouldn't be able
  // to exit early on equality and the result wouldn't even be correct.
  // A concurrent insert might occur after FindLessThan(key) but before
  // we get a chance to call Next(0).
  
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  Node* last_bigger = nullptr;

  Node* s_node;
  uint64_t s_num = 0;
  Key* scan_buf = new Key[scan_num];

  while (true) {
    assert(x != nullptr);
    Node* next = x->Next(level);
    // Make sure the lists are sorted
    assert(x == head_ || next == nullptr || KeyIsAfterNode(next->key, x));
    // Make sure we haven't overshot during our search
    assert(x == head_ || KeyIsAfterNode(key, x));
    int cmp = (next == nullptr || next == last_bigger)
        ? 1 : compare_(next->key, key);
    if (cmp == 0 || (cmp > 0 && level == 0)) {
      s_node = next;
      while (s_num < scan_num) {
        scan_buf[s_num] = next->key;
        s_node = s_node->Next(0);
        s_num++;
        if (s_node == nullptr) {
          break;
        }
      }
      return next;
    } else if (cmp < 0) {
      // Keep searching in this list
      x = next;
    } else {
      // Switch to next list, reuse compare_() result
      last_bigger = next;
      level--;
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::
  FindGreaterOrEqual_Scan(const Key& key, uint64_t scan_num) const {
  // Note: It looks like we could reduce duplication by implementing
  // this function as FindLessThan(key)->Next(0), but we wouldn't be able
  // to exit early on equality and the result wouldn't even be correct.
  // A concurrent insert might occur after FindLessThan(key) but before
  // we get a chance to call Next(0).
  Key quot = key / MOD;
  Node* x = head_;
  Node* s_node;
  
  // Array for scan operation result
  Key* scan_buf = new Key[scan_num];
  
  int level = GetMaxHeight() - 1;
  Node* last_bigger = nullptr;
  while (true) {
    assert(x != nullptr);
    Node* next = x->Next(level);
    if (next != nullptr) {
      PREFETCH(next->Next(level), 0, 1);
    }
    // Make sure the lists are sorted
    assert(x == head_ || next == nullptr || KeyIsAfterNode(next->key, x));
    // Make sure we haven't overshot during our search
    assert(x == head_ || KeyIsAfterNode(key, x));
    int cmp = (next == nullptr || next == last_bigger)
        ? 1 : compare_(next->key, quot);
    if (cmp == 0 || (cmp > 0 && level == 0)) {
      // Do scan operation
      s_node = next;
      if (s_node != nullptr) {
        int pos = ((int)key & (MOD-1)) / (MOD/(s_node->array_size));
        uint64_t s_num = 0;
        
          //printf("here2\n");
          if (compare_(s_node->ptr[pos], key) == 0) {
            // nothing to do
          } else {
            //printf("pos=%d\n", pos);
            if (pos == s_node->array_size-1) {
              s_node = s_node->Next(0);
              pos = 0;
            }
            for(int k = pos+1; k < s_node->array_size; k++) {
              //printf("k=%d\n", k);
              if (s_node->ptr[k] != 0) {
                pos = k;
                break;
              } else if (k == s_node->array_size - 1) {
                s_node = s_node->Next(0);
                pos = 0;
                break;
              }
            }
          }
        
        if (s_node == nullptr) {
          return next;
        }
        int cnt = s_node->array_size;
        for(int l = pos; l < cnt; l++) {
          //printf("here3\n");
          if (s_node->ptr[l] != 0) {
            scan_buf[s_num] = s_node->ptr[l];
            //memcpy(&scan_buf[s_num], &s_node->ptr[l], sizeof(s_node->ptr[l]));
            s_num++;
          }
        }

        while (s_num < scan_num) {
          if (s_node->Next(0) == nullptr) {
            break;
          }
          int cnt2 = s_node->array_size;
          for (int j = 0; j < cnt2; j++) {
            //printf("here5\n");
            if (s_node->ptr[j] != 0) {
              scan_buf[s_num] = s_node->ptr[j];
              //memcpy(&scan_buf[s_num], &s_node->ptr[j], sizeof(s_node->ptr[j]));
              s_num++;
              if (s_num == scan_num)
                break;
            }
          }
        }
      }
      delete[] scan_buf;
      return next;
    } else if (cmp < 0) {
      // Keep searching in this list
      x = next;
    } else {
      // Switch to next list, reuse compare_() result
      last_bigger = next;
      level--;
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::
  FindGreaterOrEqual_Buf(const Key& key) const {
  // Note: It looks like we could reduce duplication by implementing
  // this function as FindLessThan(key)->Next(0), but we wouldn't be able
  // to exit early on equality and the result wouldn't even be correct.
  // A concurrent insert might occur after FindLessThan(key) but before
  // we get a chance to call Next(0).
  Key quot = key / MOD;
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  Node* last_bigger = nullptr;
  while (true) {
    assert(x != nullptr);
    Node* next = x->Next(level);
    if (next != nullptr) {
      PREFETCH(next->Next(level), 0, 1);
    }
    // Make sure the lists are sorted
    assert(x == head_ || next == nullptr || KeyIsAfterNode(next->key, x));
    // Make sure we haven't overshot during our search
    assert(x == head_ || KeyIsAfterNode(key, x));
    int cmp = (next == nullptr || next == last_bigger)
        ? 1 : compare_(next->key, quot);
    if (cmp == 0 || (cmp > 0 && level == 0)) {
      return next;
    } else if (cmp < 0) {
      // Keep searching in this list
      x = next;
    } else {
      // Switch to next list, reuse compare_() result
      last_bigger = next;
      level--;
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node*
SkipList<Key, Comparator>::FindLessThan(const Key& key, Node** prev) const {
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  // KeyIsAfter(key, last_not_after) is definitely false
  Node* last_not_after = nullptr;
  while (true) {
    assert(x != nullptr);
    Node* next = x->Next(level);
    assert(x == head_ || next == nullptr || KeyIsAfterNode_plus(next->key, x, level));
    assert(x == head_ || KeyIsAfterNode_plus(key, x, level));
    if (next != last_not_after && KeyIsAfterNode_plus(key, next, level)) {
      // Keep searching in this list
      x = next;
    } else {
      if (prev != nullptr) {
        prev[level] = x;
      }
      if (level == 0) {
        return x;
      } else {
        // Switch to next list, reuse KeyIUsAfterNode() result
        last_not_after = next;
        level--;
      }
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node*
SkipList<Key, Comparator>::FindLessThan_plus(const Key& key, Node** prev) const {
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  // KeyIsAfter(key, last_not_after) is definitely false
  Node* last_not_after = nullptr;
  while (true) {
    assert(x != nullptr);
    Node* next = x->Next_plus(level, 0);
    assert(x == head_ || next == nullptr || KeyIsAfterNode(next->keys[level][0], x));
    assert(x == head_ || KeyIsAfterNode(key, x));
    if (next != last_not_after && KeyIsAfterNode(key, next)) {
      // Keep searching in this list
      x = next;
    } else {
      if (prev != nullptr) {
        prev[level] = x;
      }
      if (level == 0) {
        return x;
      } else {
        // Switch to next list, reuse KeyIUsAfterNode() result
        last_not_after = next;
        level--;
      }
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node*
SkipList<Key, Comparator>::FindLessOrEqual_Buf(const Key& key, Node** prev) const {
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  // KeyIsAfter(key, last_not_after) is definitely false
  Node* last_not_after = nullptr;
  //printf("key = %lu\n", key);
  while (true) {
    assert(x != nullptr);
    Node* next = x->Next(level);

    if (next != nullptr && compare_(key, next->key) == 0) {
      return next; // Signal.Jin
    }
    
    assert(x == head_ || next == nullptr || KeyIsAfterNode(next->key, x));
    assert(x == head_ || KeyIsAfterNode(key, x));
    if (next != last_not_after && KeyIsAfterNode(key, next)) {
      // Keep searching in this list
      x = next;
    } else {
      if (prev != nullptr) {
        prev[level] = x;
      }
      if (level == 0) {
        return nullptr;
      } else {
        // Switch to next list, reuse KeyIUsAfterNode() result
        last_not_after = next;
        level--;
      }
    }
  }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::FindLast()
    const {
  Node* x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    Node* next = x->Next(level);
    if (next == nullptr) {
      if (level == 0) {
        return x;
      } else {
        // Switch to next list
        level--;
      }
    } else {
      x = next;
    }
  }
}

template <typename Key, class Comparator>
uint64_t SkipList<Key, Comparator>::EstimateCount(const Key& key) const {
  uint64_t count = 0;

  Node* x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    assert(x == head_ || compare_(x->key, key) < 0);
    Node* next = x->Next(level);
    if (next == nullptr || compare_(next->key, key) >= 0) {
      if (level == 0) {
        return count;
      } else {
        // Switch to next list
        count *= kBranching_;
        level--;
      }
    } else {
      x = next;
      count++;
    }
  }
}

template <typename Key, class Comparator>
uint64_t SkipList<Key, Comparator>::EstimateNode() const {
  uint64_t count = 1;

  Node* x = head_;
  int level = GetMaxHeight() - 1;
  //printf("Max Height = %d\n", level);
  while (level >= 0) {
    Node* next = x->Next(level);
    if (next != nullptr) {
      x = next;
      ++count;
    } else if (next == nullptr) {
      printf("Height = %d, Node = %lu\n", level, count);
      level--;
      x = head_;
      count = 1;
    }
  }
  return count;
}

template <typename Key, class Comparator>
uint64_t SkipList<Key, Comparator>::Estimate_Max() const {
  uint64_t count = 0;
  uint64_t low_list = 0;

  Node* x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    Node* next = x->Next(level);
    if (next == nullptr) {
      level--;
      x = head_;
      if (level < 0)
        break;
    } else {
      count++;
      x = next;
    }
    if (level == 0)
      low_list++;
  }
  printf("skiplist size = %lu\n", (count+GetMaxHeight())*16);
  printf("Buf size = %lu\n", (low_list+1) * MOD * 8);

  return 0;
} // Signal.Jin

template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(const Comparator cmp, Allocator* allocator,
                                    int32_t max_height,
                                    int32_t branching_factor)
    : kMaxHeight_(static_cast<uint16_t>(max_height)),
      kBranching_(static_cast<uint16_t>(branching_factor)),
      kScaledInverseBranching_((Random::kMaxNext + 1) / kBranching_),
      compare_(cmp),
      allocator_(allocator),
      head_(NewNode(0 /* any key will do */, max_height)),
      max_height_(1),
      prev_height_(1) {
  assert(max_height > 0 && kMaxHeight_ == static_cast<uint32_t>(max_height));
  assert(branching_factor > 0 &&
         kBranching_ == static_cast<uint32_t>(branching_factor));
  assert(kScaledInverseBranching_ > 0);
  // Allocate the prev_ Node* array, directly from the passed-in allocator.
  // prev_ does not need to be freed, as its life cycle is tied up with
  // the allocator as a whole.
  prev_ = reinterpret_cast<Node**>(
            allocator_->AllocateAligned(sizeof(Node*) * kMaxHeight_));
  /*for (int i = 0; i < kMaxHeight_; i++) {
    head_->SetNext(i, nullptr);
    prev_[i] = head_;
  }*/

  for (int i = 0; i < ARR_SIZE; i++) {
    head_->keys[i] = new Key();
  }
  head_->Nkey[0] = new int(0);

  for (int i = 0; i < kMaxHeight_; i++) {
    head_->SetNext_plus(i, nullptr, 0);
    prev_[i] = head_;
  } // Signal.Jin

  {
    MOD = 2048;
    log_mod_ = log2_for_power_of_2(MOD);
  }
  
}

template<typename Key, class Comparator>
unsigned int SkipList<Key, Comparator>::log2_for_power_of_2(unsigned int val) {
  return 31 - __builtin_clz(val);
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::Insert(const Key& key) {
  // fast path for sequential insertion
  if (!KeyIsAfterNode(key, prev_[0]->NoBarrier_Next(0)) &&
      (prev_[0] == head_ || KeyIsAfterNode(key, prev_[0]))) {
    assert(prev_[0] != head_ || (prev_height_ == 1 && GetMaxHeight() == 1));

    // Outside of this method prev_[1..max_height_] is the predecessor
    // of prev_[0], and prev_height_ refers to prev_[0].  Inside Insert
    // prev_[0..max_height - 1] is the predecessor of key.  Switch from
    // the external state to the internal
    for (int i = 1; i < prev_height_; i++) {
      prev_[i] = prev_[0];
    }
  } else {
    // TODO(opt): we could use a NoBarrier predecessor search as an
    // optimization for architectures where memory_order_acquire needs
    // a synchronization instruction.  Doesn't matter on x86
    FindLessThan(key, prev_);
  }

  // Our data structure does not allow duplicate insertion
  assert(prev_[0]->Next(0) == nullptr || !Equal(key, prev_[0]->Next(0)->key));

  int height = RandomHeight(); // Height is defined randomly - Lee Jeyeon.
  if (height > GetMaxHeight()) { // Change Total skiplist heigth - Lee Jeyeon.
    for (int i = GetMaxHeight(); i < height; i++) {
      prev_[i] = head_;
    }
    //fprintf(stderr, "Change height from %d to %d\n", max_height_, height);

    // It is ok to mutate max_height_ without any synchronization
    // with concurrent readers.  A concurrent reader that observes
    // the new value of max_height_ will see either the old value of
    // new level pointers from head_ (nullptr), or a new value set in
    // the loop below.  In the former case the reader will
    // immediately drop to the next level since nullptr sorts after all
    // keys.  In the latter case the reader will use the new node.
    max_height_.store(height, std::memory_order_relaxed);
  }
  
  Node* x = NewNode(key, height);
  for (int i = 0; i < height; i++) {
    // NoBarrier_SetNext() suffices since we will add a barrier when
    // we publish a pointer to "x" in prev[i].
    x->NoBarrier_SetNext(i, prev_[i]->NoBarrier_Next(i));
    prev_[i]->SetNext(i, x);
  }

  prev_[0] = x;
  prev_height_ = height;
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::Insert_Buf(const Key& key) {
  // fast path for sequential insertion
  Key quot = key >> log_mod_;
  int remain = (int)key & (MOD - 1);
  Node* loc = nullptr;

  //printf("%lu, %lu, %d\n", key, quot, remain);

  if (!KeyIsAfterNode(quot, prev_[0]->NoBarrier_Next(0)) &&
      (prev_[0] == head_ || KeyIsAfterNode(quot, prev_[0]))) {
    assert(prev_[0] != head_ || (prev_height_ == 1 && GetMaxHeight() == 1));
    // Outside of this method prev_[1..max_height_] is the predecessor
    // of prev_[0], and prev_height_ refers to prev_[0].  Inside Insert
    // prev_[0..max_height - 1] is the predecessor of key.  Switch from
    // the external state to the internal
    for (int i = 1; i < prev_height_; i++) {
      prev_[i] = prev_[0];
    }
  } else {
    // TODO(opt): we could use a NoBarrier predecessor search as an
    // optimization for architectures where memory_order_acquire needs
    // a synchronization instruction.  Doesn't matter on x86
    //loc = FindLessOrEqual_Buf(quot, prev_);
  }
  loc = FindLessOrEqual_Buf(quot, prev_);
  if (loc != nullptr && compare_(loc->key, quot) == 0) {
    //int pos = remain / (MOD/loc->array_size); 
    int pos = remain >> log2_for_power_of_2(MOD >> log2_for_power_of_2(loc->array_size));
    //printf("key=%lu, quot = %lu, remain=%d, pos=%d, as=%d\n", key, quot, remain, pos, loc->array_size);
    if (compare_(loc->ptr[pos],0) == 0) {
      loc->ptr[pos] = key;
    } else if (compare_(loc->ptr[pos], key) ==  0) {
      return;
    } else {
      int re_array_size = MOD >> (int)floor(log2(abs((int)(loc->ptr[pos]) - (int)key)));
      int div_shift_bit = log2_for_power_of_2(MOD >> log2_for_power_of_2(re_array_size));

      //printf("loc->key=%lu, key=%lu, re=%d, div=%d\n", loc->key, key, re_array_size, div_shift_bit);
      //printf("abs=%d\n", abs((int)(loc->key) - (int)key));
      //printf("log2=%d\n", (int)log2(abs((int)(loc->key) - (int)key)));

      Key *buf2 = new Key[re_array_size]();
      for(int j = 0; j < loc->array_size; j++) {
        if (compare_(loc->ptr[j],0) != 0) {
          int pos2 = ((int)loc->ptr[j] & (MOD-1)) >> div_shift_bit;
          buf2[pos2] = loc->ptr[j];
        }
      }
      loc->array_size = re_array_size;
      delete [] loc->ptr;
      loc->ptr = buf2;

      pos = remain >> div_shift_bit;
      if (compare_(loc->ptr[pos], 0) == 0) {
        loc->ptr[pos] = key;
      }
    }
    //if (loc->fanout > 32) printf("Array size overflow\n");
    return;
  } // Signal.Jin
  //printf("key=%lu, quot=%lu\n", key, quot);

  // Our data structure does not allow duplicate insertion
  assert(prev_[0]->Next(0) == nullptr || !Equal(key, prev_[0]->Next(0)->key));

  int height = RandomHeight(); // Height is defined randomly - Lee Jeyeon.
  if (height > GetMaxHeight()) { // Change Total skiplist heigth - Lee Jeyeon.
    for (int i = GetMaxHeight(); i < height; i++) {
      prev_[i] = head_;
    }
    //fprintf(stderr, "Change height from %d to %d\n", max_height_, height);

    // It is ok to mutate max_height_ without any synchronization
    // with concurrent readers.  A concurrent reader that observes
    // the new value of max_height_ will see either the old value of
    // new level pointers from head_ (nullptr), or a new value set in
    // the loop below.  In the former case the reader will
    // immediately drop to the next level since nullptr sorts after all
    // keys.  In the latter case the reader will use the new node.
    max_height_.store(height, std::memory_order_relaxed);
  }
  Node* x = NewNode(quot, height); //printf("[2] %lu, %lu, %lu\n", x->key, key, next->key);
  for (int i = 0; i < height; i++) {
    // NoBarrier_SetNext() suffices since we will add a barrier when
    // we publish a pointer to "x" in prev[i].
    x->NoBarrier_SetNext(i, prev_[i]->NoBarrier_Next(i));
    prev_[i]->SetNext(i, x);
  }

  { // Signal.Jin
    Key *buf = new Key[4]();
    x->array_size = 4;
    int pos = remain / (MOD/x->array_size);
    buf[pos] = key;
    x->ptr = buf;
  }

  prev_[0] = x;
  prev_height_ = height;
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::Insert_plus(const Key& key) {
  // fast path for sequential insertion
  /*if (!KeyIsAfterNode(key, prev_[0]->NoBarrier_Next(0)) &&
      (prev_[0] == head_ || KeyIsAfterNode(key, prev_[0]))) {
    assert(prev_[0] != head_ || (prev_height_ == 1 && GetMaxHeight() == 1));

    // Outside of this method prev_[1..max_height_] is the predecessor
    // of prev_[0], and prev_height_ refers to prev_[0].  Inside Insert
    // prev_[0..max_height - 1] is the predecessor of key.  Switch from
    // the external state to the internal
    for (int i = 1; i < prev_height_; i++) {
      prev_[i] = prev_[0];
    }
  } else {
    // TODO(opt): we could use a NoBarrier predecessor search as an
    // optimization for architectures where memory_order_acquire needs
    // a synchronization instruction.  Doesn't matter on x86
    FindLessThan_plus(key, prev_);
  }*/
  printf("Here1\n");
  FindLessThan_plus(key, prev_);
  printf("Here2\n");
  // Our data structure does not allow duplicate insertion
  assert(prev_[0]->Next_plus(0, 0) == nullptr || !Equal(key, prev_[0]->Next_plus(0, 0)->keys[0][0]));
  printf("Here4\n");
  if (prev_[0]->Nkey[0][0] != 0 && prev_[0]->Nkey[0][0] < ARR_SIZE) {
    printf("Here3\n");
    for(int i = 0; i < prev_[0]->Nkey[0][0]; i++) {
      if (prev_[0]->keys[0][i] == key) {
        return;
      } else if (prev_[0]->keys[0][i+1] != (Key)0) {
        if (prev_[0]->keys[0][i] < key && prev_[0]->keys[0][i+1] > key) {
          for(int j = ARR_SIZE - 2; j >= i+1; j--) {
            prev_[0]->keys[0][j+1] = prev_[0]->keys[0][j];
          }
          prev_[0]->keys[0][i+1] = key;
          // If array is full, do increase height;
        }
        return;
      }
    }
    prev_[0]->keys[0][prev_[0]->Nkey[0][0]] = key;
    // If array is full, do increase height;
    return;
  }

  // Deterministic Height - Signal.Jin
  int height = 1;
  
  Node* x = NewNode(key, height);
  x->NoBarrier_SetNext_plus(0, prev_[0]->NoBarrier_Next_plus(0, 0), 0);
  prev_[0]->SetNext_plus(0, x, 0);

  { // Signal.Jin
    Key temp[4] = {key, 0, 0, 0};
    int Nkey_[1] = {1};
    x->keys[0] = temp;
    x->Nkey[0] = Nkey_;
  }

  prev_[0] = x;
  prev_height_ = height;
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key& key) const {
  Node* x = FindGreaterOrEqual(key);
  if (x != nullptr && Equal(key, x->key)) {
    return true;
  } else {
    return false;
  }
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains_OriScan(const Key& key, uint64_t scan_num) const {
  Node* x = FindGreaterOrEqual_OriScan(key, scan_num);
  if (x != nullptr && Equal(key, x->key)) {
    return true;
  } else {
    return false;
  }
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains_Scan(const Key& key, uint64_t scan_num) const {
  Node* x = FindGreaterOrEqual_Scan(key, scan_num);
  Key res = 0;
  if (x != nullptr) {
    int pos = ((int)key & (MOD-1)) / (MOD/(x->array_size));
    res = x->ptr[pos];
  }

  if (x != nullptr && Equal(key, res)) {
    return true;
  } else {
    return false;
  }
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains_Buf(const Key& key) const {
  Node* x = FindGreaterOrEqual_Buf(key);
  Key res = 0;

  if (x != nullptr) {
    int pos = ((int)key & (MOD-1)) / (MOD/(x->array_size));
    res = x->ptr[pos];
  }

  //printf("key=%lu, res=%lu, fanout=%d, pos=%d\n", key, res, x->fanout, pos);
  //for(int i = 0; i < x->fanout; i++) {
  //  printf("%lu ", x->ptr[i]);
  //}
  //printf("\n");

  if (x != nullptr && Equal(key, res)) {
    return true;
  } else {
    return false;
  }
}

}  // namespace ROCKSDB_NAMESPACE
