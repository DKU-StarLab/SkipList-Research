//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "memtable/inlineskiplist.h"

#include <set>
#include <unordered_set>

#include "memory/concurrent_arena.h"
#include "rocksdb/env.h"
#include "test_util/testharness.h"
#include "util/hash.h"
#include "util/random.h"

namespace ROCKSDB_NAMESPACE {

// Our test skip list stores 8-byte unsigned integers
using Key = uint64_t;

static const char* Encode(const uint64_t* key) {
  return reinterpret_cast<const char*>(key);
}

static Key Decode(const char* key) {
  Key rv;
  memcpy(&rv, key, sizeof(Key));
  return rv;
}

struct TestComparator {
  using DecodedType = Key;

  static DecodedType decode_key(const char* b) { return Decode(b); }

  int operator()(const char* a, const char* b) const {
    if (Decode(a) < Decode(b)) {
      return -1;
    } else if (Decode(a) > Decode(b)) {
      return +1;
    } else {
      return 0;
    }
  }

  int operator()(const char* a, const DecodedType b) const {
    if (Decode(a) < b) {
      return -1;
    } else if (Decode(a) > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

using TestInlineSkipList = InlineSkipList<TestComparator>;

class InlineSkipTest : public testing::Test {
 public:
  void Insert(TestInlineSkipList* list, Key key) {
    char* buf = list->AllocateKey(sizeof(Key));
    memcpy(buf, &key, sizeof(Key));
    list->Insert(buf);
    keys_.insert(key);
  }

  bool InsertWithHint(TestInlineSkipList* list, Key key, void** hint) {
    char* buf = list->AllocateKey(sizeof(Key));
    memcpy(buf, &key, sizeof(Key));
    bool res = list->InsertWithHint(buf, hint);
    keys_.insert(key);
    return res;
  }

  void Validate(TestInlineSkipList* list) {
    // Check keys exist.
    for (Key key : keys_) {
      ASSERT_TRUE(list->Contains(Encode(&key)));
    }
    // Iterate over the list, make sure keys appears in order and no extra
    // keys exist.
    TestInlineSkipList::Iterator iter(list);
    ASSERT_FALSE(iter.Valid());
    Key zero = 0;
    iter.Seek(Encode(&zero));
    for (Key key : keys_) {
      ASSERT_TRUE(iter.Valid());
      ASSERT_EQ(key, Decode(iter.key()));
      iter.Next();
    }
    ASSERT_FALSE(iter.Valid());
    // Validate the list is well-formed.
    list->TEST_Validate();
  }

 private:
  std::set<Key> keys_;
};

TEST_F(InlineSkipTest, Empty) {
  Arena arena;
  TestComparator cmp;
  InlineSkipList<TestComparator> list(cmp, &arena);
  Key key = 10;
  ASSERT_TRUE(!list.Contains(Encode(&key)));

  InlineSkipList<TestComparator>::Iterator iter(&list);
  ASSERT_TRUE(!iter.Valid());
  iter.SeekToFirst();
  ASSERT_TRUE(!iter.Valid());
  key = 100;
  iter.Seek(Encode(&key));
  ASSERT_TRUE(!iter.Valid());
  iter.SeekForPrev(Encode(&key));
  ASSERT_TRUE(!iter.Valid());
  iter.SeekToLast();
  ASSERT_TRUE(!iter.Valid());
}

TEST_F(InlineSkipTest, InsertAndLookup) {
  const int N = 1;
  const int R = 1;
  Random rnd(1000);
  std::set<Key> keys;
  ConcurrentArena arena;
  TestComparator cmp;
  InlineSkipList<TestComparator> list(cmp, &arena);
  for (int i = 1; i < N+1; i++) {
    Key key = rnd.Next() % N + 513;
    //printf("Key = %lu\n", key);
    //Key key = i+512;
    if (keys.insert(key).second) {
      char* buf = list.AllocateKey(sizeof(Key)); // Allocate memory address
      memcpy(buf, &key, sizeof(Key)); // Store key data
      //printf("Insert\n");
      //list.InsertConcurrently_Array(buf);
      list.InsertConcurrently(buf);
    }
  }
  //printf("Lookup\n");
  
  for (Key i = 1; i < R+1; i++) {
    Key Lkey = rnd.Next() % R + 513;
    //Key Lkey = i+512;
    //printf("Lookup\n");
    if (list.Contains(Encode(&Lkey))) {
      ASSERT_EQ(keys.count(Lkey), 1U);
    } else {
      ASSERT_EQ(keys.count(Lkey), 0U);
    }
  }
}

#if !defined(ROCKSDB_VALGRIND_RUN) || defined(ROCKSDB_FULL_VALGRIND_RUN)
// We want to make sure that with a single writer and multiple
// concurrent readers (with no synchronization other than when a
// reader's iterator is created), the reader always observes all the
// data that was present in the skip list when the iterator was
// constructor.  Because insertions are happening concurrently, we may
// also observe new values that were inserted since the iterator was
// constructed, but we should never miss any values that were present
// at iterator construction time.
//
// We generate multi-part keys:
//     <key,gen,hash>
// where:
//     key is in range [0..K-1]
//     gen is a generation number for key
//     hash is hash(key,gen)
//
// The insertion code picks a random key, sets gen to be 1 + the last
// generation number inserted for that key, and sets hash to Hash(key,gen).
//
// At the beginning of a read, we snapshot the last inserted
// generation number for each key.  We then iterate, including random
// calls to Next() and Seek().  For every key we encounter, we
// check that it is either expected given the initial snapshot or has
// been concurrently added since the iterator started.
class ConcurrentTest {
 public:
  static const uint32_t K = 8;

 private:
  static uint64_t key(Key key) { return (key >> 40); }
  static uint64_t gen(Key key) { return (key >> 8) & 0xffffffffu; }
  static uint64_t hash(Key key) { return key & 0xff; }

  static uint64_t HashNumbers(uint64_t k, uint64_t g) {
    uint64_t data[2] = {k, g};
    return Hash(reinterpret_cast<char*>(data), sizeof(data), 0);
  }

  static Key MakeKey(uint64_t k, uint64_t g) {
    assert(sizeof(Key) == sizeof(uint64_t));
    assert(k <= K);  // We sometimes pass K to seek to the end of the skiplist
    assert(g <= 0xffffffffu);
    return ((k << 40) | (g << 8) | (HashNumbers(k, g) & 0xff));
  }

  static bool IsValidKey(Key k) {
    return hash(k) == (HashNumbers(key(k), gen(k)) & 0xff);
  }

  static Key RandomTarget(Random* rnd) {
    switch (rnd->Next() % 10) {
      case 0:
        // Seek to beginning
        return MakeKey(0, 0);
      case 1:
        // Seek to end
        return MakeKey(K, 0);
      default:
        // Seek to middle
        return MakeKey(rnd->Next() % K, 0);
    }
  }

  // Per-key generation
  struct State {
    std::atomic<int> generation[K];
    void Set(int k, int v) {
      generation[k].store(v, std::memory_order_release);
    }
    int Get(int k) { return generation[k].load(std::memory_order_acquire); }

    State() {
      for (unsigned int k = 0; k < K; k++) {
        Set(k, 0);
      }
    }
  };

  // Current state of the test
  State current_;

  ConcurrentArena arena_;

  // InlineSkipList is not protected by mu_.  We just use a single writer
  // thread to modify it.
  InlineSkipList<TestComparator> list_;

 public:
  ConcurrentTest() : list_(TestComparator(), &arena_) {}

  // REQUIRES: No concurrent calls to WriteStep or ConcurrentWriteStep
  void WriteStep(Random* rnd) {
    const uint32_t k = rnd->Next() % K;
    const int g = current_.Get(k) + 1;
    const Key new_key = MakeKey(k, g);
    //printf("new_key = %lu\n", new_key);
    char* buf = list_.AllocateKey(sizeof(Key));
    memcpy(buf, &new_key, sizeof(Key));
    list_.Insert(buf);
    //list_.InsertConcurrently_Array(buf); // Signal.Jin 
    current_.Set(k, g);
  }

  // REQUIRES: No concurrent calls for the same k
  void ConcurrentWriteStep(uint32_t k, bool use_hint = false) {
    Random rnd(k);
    const int g = current_.Get(k) + 1;
    const Key new_key = MakeKey(k, g);
    //const Key new_key = rnd.Next() % 512000 + 513;
    //printf("new_key = %lu\n", new_key);
    char* buf = list_.AllocateKey(sizeof(Key));
    memcpy(buf, &new_key, sizeof(Key));
    if (use_hint) {
      void* hint = nullptr;
      list_.InsertWithHintConcurrently(buf, &hint);
      delete[] reinterpret_cast<char*>(hint);
    } else {
      list_.InsertConcurrently(buf);
    }
    ASSERT_EQ(g, current_.Get(k) + 1);
    current_.Set(k, g);
  }

  void ConcurrentWriteStep_Array(uint32_t k, const int N, bool use_hint = false) {
    Random rnd2(5326);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, N);

    const int g = current_.Get(k) + 1;
    //const Key new_key = MakeKey(k, g);
    const Key new_key = distr(gen) + 513;
    //printf("new_key = %lu\n", new_key);
    char* buf = list_.AllocateKey(sizeof(Key));
    memcpy(buf, &new_key, sizeof(Key));
    if (use_hint) {
      void* hint = nullptr;
      list_.InsertWithHintConcurrently(buf, &hint);
      delete[] reinterpret_cast<char*>(hint);
    } else {
      list_.InsertConcurrently_Array(buf);
    }
    ASSERT_EQ(g, current_.Get(k) + 1);
    current_.Set(k, g);
  }

  void ReadStep(Random* rnd) {
    // Remember the initial committed state of the skiplist.
    State initial_state;
    for (unsigned int k = 0; k < K; k++) {
      initial_state.Set(k, current_.Get(k));
    }

    Key pos = RandomTarget(rnd);
    //printf("[pos] = %lu\n", pos);
    InlineSkipList<TestComparator>::Iterator iter(&list_);
    //iter.Seek(Encode(&pos));
    iter.Seek(Encode(&pos)); // Signal.Jin
    while (true) {
      Key current;
      if (!iter.Valid()) {
        current = MakeKey(K, 0);
      } else {
        current = Decode(iter.key());
        //current = iter.key_array(pos); // Signal.Jin
        //printf("current = %lu, pos = %lu\n", current, pos);
        ASSERT_TRUE(IsValidKey(current)) << current;
      }
      ASSERT_LE(pos, current) << "should not go backwards";

      // Verify that everything in [pos,current) was not present in
      // initial_state.
      while (pos < current) {
        ASSERT_LT(key(pos), K) << pos;

        // Note that generation 0 is never inserted, so it is ok if
        // <*,0,*> is missing.
        ASSERT_TRUE((gen(pos) == 0U) ||
                    (gen(pos) > static_cast<uint64_t>(initial_state.Get(
                                    static_cast<int>(key(pos))))))
            << "key: " << key(pos) << "; gen: " << gen(pos)
            << "; initgen: " << initial_state.Get(static_cast<int>(key(pos)));

        // Advance to next key in the valid key space
        if (key(pos) < key(current)) {
          pos = MakeKey(key(pos) + 1, 0);
        } else {
          pos = MakeKey(key(pos), gen(pos) + 1);
        }
      }

      if (!iter.Valid()) {
        //printf("break\n");
        break;
      }

      if (rnd->Next() % 2) {
        iter.Next();
        pos = MakeKey(key(pos), gen(pos) + 1);
      } else {
        Key new_target = RandomTarget(rnd);
        if (new_target > pos) {
          pos = new_target;
          iter.Seek(Encode(&new_target));
          //iter.Seek_Array(Encode(&new_target)); // Signal.Jin
        }
      }
    }
  }

  void ReadStep_Array(const int N) {
    // Remember the initial committed state of the skiplist.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, N);

    State initial_state;
    for (unsigned int k = 0; k < K; k++) {
      initial_state.Set(k, current_.Get(k));
    }

    const Key new_key = distr(gen) + 513;
    char* buf = list_.AllocateKey(sizeof(Key));
    memcpy(buf, &new_key, sizeof(Key));
    list_.Contains_Array(buf);
  }
};
const uint32_t ConcurrentTest::K;

// Simple test that does single-threaded testing of the ConcurrentTest
// scaffolding.

class TestState {
 public:
  ConcurrentTest t_;
  bool use_hint_;
  int seed_;
  std::atomic<bool> quit_flag_;
  std::atomic<uint32_t> next_writer_;

  enum ReaderState { STARTING, RUNNING, DONE };

  explicit TestState(int s)
      : seed_(s),
        quit_flag_(false),
        state_(STARTING),
        pending_writers_(0),
        state_cv_(&mu_) {}

  void Wait(ReaderState s) {
    mu_.Lock();
    while (state_ != s) {
      state_cv_.Wait();
    }
    mu_.Unlock();
  }

  void Change(ReaderState s) {
    mu_.Lock();
    state_ = s;
    state_cv_.Signal();
    mu_.Unlock();
  }

  void AdjustPendingWriters(int delta) {
    mu_.Lock();
    pending_writers_ += delta;
    if (pending_writers_ == 0) {
      state_cv_.Signal();
    }
    mu_.Unlock();
  }

  void WaitForPendingWriters() {
    mu_.Lock();
    while (pending_writers_ != 0) {
      state_cv_.Wait();
    }
    mu_.Unlock();
  }

 private:
  port::Mutex mu_;
  ReaderState state_;
  int pending_writers_;
  port::CondVar state_cv_;
};

static void ConcurrentReader(void* arg) {
  TestState* state = reinterpret_cast<TestState*>(arg);
  Random rnd(state->seed_);
  int64_t reads = 0;
  //printf("Do read\n");
  state->Change(TestState::RUNNING);
  while (!state->quit_flag_.load(std::memory_order_acquire)) {
    //printf("Do read\n");
    state->t_.ReadStep(&rnd);
    ++reads;
  }
  state->Change(TestState::DONE);
}

static void ConcurrentReader_Array(void* arg) { // Signal.Jin
  TestState* state = reinterpret_cast<TestState*>(arg);
  Random rnd(state->seed_);
  const int N = 4000000;
  int64_t reads = 0;
  state->Change(TestState::RUNNING);
  while (!state->quit_flag_.load(std::memory_order_acquire)) {
    state->t_.ReadStep_Array(N);
    ++reads;
  }
  state->Change(TestState::DONE);
}

static void ConcurrentWriter(void* arg) {
  TestState* state = reinterpret_cast<TestState*>(arg);
  uint32_t k = state->next_writer_++ % ConcurrentTest::K;
  state->t_.ConcurrentWriteStep(k, state->use_hint_);
  state->AdjustPendingWriters(-1);
}

static void ConcurrentWriter_Array(void* arg) {
  TestState* state = reinterpret_cast<TestState*>(arg);
  uint32_t k = state->next_writer_++ % ConcurrentTest::K;
  const int N = 4000000;
  state->t_.ConcurrentWriteStep_Array(k, N, state->use_hint_);
  state->AdjustPendingWriters(-1);
}

static void RunConcurrentRead(int run) {
  const int seed = test::RandomSeed() + (run * 100);
  Random rnd(seed);
  const int N = 0;
  const int kSize = 0;
  for (int i = 0; i < N; i++) {
    if ((i % 100) == 0) {
      fprintf(stderr, "Run %d of %d\n", i, N);
    }
    TestState state(seed + 1);
    Env::Default()->SetBackgroundThreads(1);
    Env::Default()->Schedule(ConcurrentReader, &state);
    state.Wait(TestState::RUNNING);
    for (int k = 0; k < kSize; ++k) {
      //printf("Execute\n");
      state.t_.WriteStep(&rnd);
    }
    state.quit_flag_.store(true, std::memory_order_release);
    state.Wait(TestState::DONE);
  }
}

static void RunConcurrentInsert(int run, bool use_hint = false,
                                int write_parallelism = 16) {
  Env::Default()->SetBackgroundThreads(1 + write_parallelism,
                                       Env::Priority::LOW);
  const int seed = test::RandomSeed() + (run * 100);
  Random rnd(seed);
  const int N = 1;
  const int kSize = 4000000;
  for (int i = 0; i < N; i++) {
    if ((i % 100) == 0) {
      fprintf(stderr, "Run %d of %d\n", i, N);
    }
    TestState state(seed + 1);
    state.use_hint_ = use_hint;
    Env::Default()->Schedule(ConcurrentReader, &state);
    state.Wait(TestState::RUNNING);
    for (int k = 0; k < kSize; k += write_parallelism) {
      state.next_writer_ = rnd.Next();
      state.AdjustPendingWriters(write_parallelism);
      for (int p = 0; p < write_parallelism; ++p) {
        Env::Default()->Schedule(ConcurrentWriter, &state);
      }
      state.WaitForPendingWriters();
    }
    state.quit_flag_.store(true, std::memory_order_release);
    state.Wait(TestState::DONE);
  }
}

static void RunConcurrentRead_Array(int run) {
  const int seed = test::RandomSeed() + (run * 100);
  Random rnd(seed);
  const int N = 0;
  const int kSize = 0;
  for (int i = 0; i < N; i++) {
    if ((i % 100) == 0) {
      fprintf(stderr, "Run %d of %d\n", i, N);
    }
    TestState state(seed + 1);
    Env::Default()->SetBackgroundThreads(1);
    Env::Default()->Schedule(ConcurrentReader_Array, &state);
    state.Wait(TestState::RUNNING);
    for (int k = 0; k < kSize; ++k) {
      //printf("Execute\n");
      state.t_.WriteStep(&rnd);
    }
    state.quit_flag_.store(true, std::memory_order_release);
    state.Wait(TestState::DONE);
  }
}

static void RunConcurrentInsert_Array(int run, bool use_hint = false,
                                int write_parallelism = 16) {
  Env::Default()->SetBackgroundThreads(1 + write_parallelism,
                                       Env::Priority::LOW);
  const int seed = test::RandomSeed() + (run * 100);
  Random rnd(seed);
  const int N = 0;
  const int kSize = 4000000;
  for (int i = 0; i < N; i++) {
    if ((i % 100) == 0) {
      fprintf(stderr, "Run %d of %d\n", i, N);
    }
    TestState state(seed + 1);
    state.use_hint_ = use_hint;
    Env::Default()->Schedule(ConcurrentReader_Array, &state);
    state.Wait(TestState::RUNNING);
    for (int k = 0; k < kSize; k += write_parallelism) {
      state.next_writer_ = rnd.Next();
      state.AdjustPendingWriters(write_parallelism);
      for (int p = 0; p < write_parallelism; ++p) {
        Env::Default()->Schedule(ConcurrentWriter_Array, &state);
      }
      state.WaitForPendingWriters();
    }
    state.quit_flag_.store(true, std::memory_order_release);
    state.Wait(TestState::DONE);
  }
}

TEST_F(InlineSkipTest, ConcurrentRead1) { RunConcurrentRead(1); }
TEST_F(InlineSkipTest, ConcurrentInsert1) { RunConcurrentInsert(1); }

TEST_F(InlineSkipTest, ConcurrentInsert_Array) { RunConcurrentInsert_Array(1); }
TEST_F(InlineSkipTest, ConcurrentRead_Array) { RunConcurrentRead_Array(1); }

#endif  // !defined(ROCKSDB_VALGRIND_RUN) || defined(ROCKSDB_FULL_VALGRIND_RUN)
}  // namespace ROCKSDB_NAMESPACE

int main(int argc, char** argv) {
  ROCKSDB_NAMESPACE::port::InstallStackTraceHandler();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
