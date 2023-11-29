#include <stdlib.h>
#include <cstdint>
#include <algorithm>
#include <cmath>

#define ARR_SIZE 128
#define MAXHEIGHT 30

typedef uint64_t Key;

int compare_(const Key& a, const Key& b) {
    if (a < b) {
        return -1;
    } else if (a > b) {
        return +1;
    } else {
        return 0;
    }
}

template<typename Key>
class SkipList {
   private:
    struct Node;

   public:
    SkipList();

    void Insert_Test2(const Key& key); // Test code for insert operation (ver2)
    bool Contains(const Key& key) const;
    void Scan(const Key& key, const int scan_num);

    void Print() const;

   private:
    int kMaxHeight_;
    Node* head_[MAXHEIGHT];
    int max_height_;

    inline int GetMaxHeight() const {
        return max_height_;
    }

    Node* NewNode(const Key& key);
    //Node* FindGreaterOrEqual(const Key& key);
    //Node* FindLessThan(const Key& key, Node** prev = nullptr) const;
};


template<typename Key>
struct SkipList<Key>::Node {
    Key keys[ARR_SIZE];
    // keys[0] = leader key of current node
    Node* forward;
    Node* next[ARR_SIZE];
    int N_key;

    Node(Key key) : N_key(1) {
        this->keys[0] = key;
        this->forward = nullptr;
        for(int i = 0; i < ARR_SIZE; i++) {
            next[i] = nullptr;
        }
    }
};

template<typename Key>
typename SkipList<Key>::Node*
SkipList<Key>::NewNode(const Key& key) {
    return new Node(key);
}

template<typename Key>
SkipList<Key>::SkipList() {
    kMaxHeight_ = MAXHEIGHT;
    max_height_ = 1;

    for(int i = 0; i < kMaxHeight_; i++) {
        head_[i] = NewNode(Key());  
    }
}

template<typename Key>
void SkipList<Key>::Insert_Test2(const Key& key) {
    Node* prev_[MAXHEIGHT];
    std::copy(std::begin(head_), std::end(head_), std::begin(prev_));
    int height = GetMaxHeight() - 1; // Using for search
    Node* x = head_[height]; // Use when searching
    while (true) { // Find the location which will insert the key using prev_ and head_
        while (x->forward != nullptr && compare_(x->forward->keys[0], key) <= 0) {
            if (compare_(x->forward->keys[0], key) == 0) {
                return; // Do not allow duplicated key
            } else {
                x = x->forward;
            }
        }
        prev_[height--] = x;
        if (height >= 0) {
            if (x == head_[height+1]) {
                x = head_[height];
            } else {
                for (int i = 0; i < x->N_key; i++) {
                    if (x->keys[i] <= key && key < x->keys[i+1]) {
                        x = x->next[i];
                        break;
                    } else if (x->keys[i+1] == 0) {
                        x = x->next[i];
                        break;
                    } else if (x->keys[ARR_SIZE-1] <= key && x->keys[ARR_SIZE-1] != 0) {
                        x = x->next[ARR_SIZE-1];
                        break;
                    }
                }
            }
        } else break;
    } // TODO:: Implement lookup operation (completed)

    // Do not allow duplicated key
    // TODO:: Need to Implement (completed)
    if (prev_[0] != head_[0]) {
        for (int i = 0; i < prev_[0]->N_key; i++) {
            if (compare_(prev_[0]->keys[i], key) == 0) {
                return;
            }
        }
    }
    
    // Do insert operation
    int level = 0;
    while (true) {
        int stop_flag = 0;
        int cur_height = GetMaxHeight() - 1;
        if (prev_[level] == head_[level] && prev_[level]->forward == nullptr) {
        // Case 1: There is no node in list and need to make a new node
            if (level == 0) { // Case 1-1: If insert into H0
                Node* Elist_node = NewNode(key);
                Elist_node->forward = prev_[level]->forward;
                prev_[level]->forward = Elist_node;
                break;
            } else { // Case 1-2: If not insert into H0
                if (cur_height < level) {
                    max_height_++;
                }
                if (prev_[level-1]->forward != nullptr && prev_[level-1] == head_[level-1]) {
                    // TODO:: Problem is here!!
                    Node* Elist_node = NewNode(prev_[level-1]->forward->keys[0]);
                    Elist_node->forward = prev_[level]->forward;
                    Elist_node->next[0] = prev_[level-1]->forward;
                    prev_[level]->forward = Elist_node;
                } else {
                    Node* Elist_node = NewNode(prev_[level-1]->keys[0]);
                    Elist_node->forward = prev_[level]->forward;
                    Elist_node->next[0] = prev_[level-1];
                    prev_[level]->forward = Elist_node;
                }
                break;
            }
        } else if (prev_[level] == head_[level]) {
            // Case 2: There is another node in list and need to make a new node
            // Additionally, a merge operation may be necessary depending on the situation
            // TODO:: Need to optimize, update upper list leader key
            if (prev_[level]->forward->N_key != ARR_SIZE) { // Case 2-1: Forward node has a room, so we need to do merge
                if (level == 0) { // Case 2-1-1: Insert into H0
                    Key update_key = prev_[level]->forward->keys[0];
                    for (int shift = prev_[level]->forward->N_key; shift > 0; shift--) {
                        prev_[level]->forward->keys[shift] = prev_[level]->forward->keys[shift-1];
                    }
                    prev_[level]->forward->keys[0] = key;
                    prev_[level]->forward->N_key++;
                    if (prev_[level]->forward->N_key == ARR_SIZE) {
                        level++;
                        if (cur_height < level) {
                            max_height_++;
                        } 
                    } else {
                        for (int update = level+1; update < GetMaxHeight(); update++) {
                            int loop = 0;
                            for (loop = 0; loop < prev_[update]->forward->N_key; loop++) {
                                if (prev_[update]->forward->keys[loop] == update_key) {
                                    prev_[update]->forward->keys[loop] = key;
                                    break;
                                }
                            }
                            if (loop != 0) break;
                        }
                        stop_flag++;
                    }
                } else { // Case 2-1-2: Not insert into H0
                    Key update_key = prev_[level]->forward->keys[0];
                    for (int shift = prev_[level]->forward->N_key; shift > 0; shift--) {
                        prev_[level]->forward->keys[shift] = prev_[level]->forward->keys[shift-1];
                        prev_[level]->forward->next[shift] = prev_[level]->forward->next[shift-1];
                    }
                    // TODO:: Need to check (add forward)
                    if (prev_[level-1] == head_[level-1]) {
                        prev_[level]->forward->keys[0] = prev_[level-1]->forward->keys[0];
                        prev_[level]->forward->next[0] = prev_[level-1]->forward;
                    } else {
                        prev_[level]->forward->keys[0] = prev_[level-1]->keys[0];
                        prev_[level]->forward->next[0] = prev_[level-1];
                    }
                    prev_[level]->forward->N_key++;
                    if (prev_[level]->forward->N_key == ARR_SIZE) {
                        level++;
                        if (cur_height < level) {
                            max_height_++;
                        }
                    } else {
                        for (int update = level+1; update < GetMaxHeight(); update++) {
                            int loop = 0;
                            for (loop = 0; loop < prev_[update]->forward->N_key; loop++) {
                                if (prev_[update]->forward->keys[loop] == update_key) {
                                    prev_[update]->forward->keys[loop] = prev_[level]->forward->keys[0];
                                    break;
                                }
                            }
                            if (loop != 0) break;
                        }
                        stop_flag++;
                    }
                }
            } else { // Case 2-2: Forward node has no room, so we need to make a new node
                if (level == 0) { // Case 2-2-1: Insert into H0
                    Node* add_node = NewNode(key);
                    add_node->forward = prev_[level]->forward;
                    prev_[level]->forward = add_node;
                    // prev_[level] = add_node;
                    // level++;
                    // if (cur_height < level) {
                    //     max_height_++;
                    // }
                    break;
                } else { // Case 2-2-2: Not insert into H0
                    if (prev_[level-1] == head_[level-1]) {
                        Node* add_node = NewNode(prev_[level-1]->forward->keys[0]);
                        add_node->forward = prev_[level]->forward;
                        add_node->next[0] = prev_[level-1]->forward;
                        prev_[level]->forward = add_node;
                    } else {
                        Node* add_node = NewNode(prev_[level-1]->keys[0]);
                        add_node->forward = prev_[level]->forward;
                        add_node->next[0] = prev_[level-1];
                        prev_[level]->forward = add_node;
                    }
                    // prev_[level] = add_node;
                    // level++;
                    // if (cur_height < level) {
                    //     max_height_++;
                    // }
                    break;
                }
            }
        } else {
            // Case 3: New node must be inserted between nodes or into prev_ node (not head_)
            // Additionally, a merge operation may be necessary depending on the situation
            if (prev_[level]->N_key != ARR_SIZE) { // Case 3-1: prev_ node has a room, so we insert into that node
                if (level == 0) { // Case 3-1-1: Insert into H0
                    for (int shift = 0; shift < prev_[level]->N_key; shift++) {
                        if (prev_[level]->keys[shift] == key) {
                            stop_flag++;
                            break;
                        } else if (prev_[level]->keys[shift] <= key && key < prev_[level]->keys[shift+1]) {
                            for (int do_shift = prev_[level]->N_key-1; do_shift > shift; do_shift--) {
                                prev_[level]->keys[do_shift+1] = prev_[level]->keys[do_shift];
                            }
                            prev_[level]->keys[shift+1] = key;
                            prev_[level]->N_key++;
                            if (prev_[level]->N_key == ARR_SIZE && prev_[level+1] == head_[level+1]) {
                                level++;
                                if (cur_height < level) {
                                    max_height_++;
                                }
                            } else stop_flag++;
                            break;
                        } else if (prev_[level]->keys[shift+1] == 0) {
                            prev_[level]->keys[shift+1] = key;
                            prev_[level]->N_key++;
                            if (prev_[level]->N_key == ARR_SIZE && prev_[level+1] == head_[level+1]) {
                                level++;
                                if (cur_height < level) {
                                    max_height_++;
                                }
                            } else stop_flag++;
                            break;
                        }
                    }
                } else { // Case 3-1-2: Not insert into H0
                    for (int shift = 0; shift < prev_[level]->N_key; shift++) {
                        if (prev_[level]->keys[shift] == prev_[level-1]->keys[0]) {
                            stop_flag++;
                            break;
                        } else if (prev_[level]->keys[shift] < prev_[level-1]->keys[0] && prev_[level-1]->keys[0] < prev_[level]->keys[shift+1]) {
                            for (int do_shift = prev_[level]->N_key-1; do_shift > shift; do_shift--) {
                                prev_[level]->keys[do_shift+1] = prev_[level]->keys[do_shift];
                                prev_[level]->next[do_shift+1] = prev_[level]->next[do_shift];
                            }
                            prev_[level]->keys[shift+1] = prev_[level-1]->keys[0];
                            prev_[level]->next[shift+1] = prev_[level-1];
                            prev_[level]->N_key++;
                            if (prev_[level]->N_key == ARR_SIZE && prev_[level+1] == head_[level+1]) {
                                level++;
                                if (cur_height < level) {
                                    max_height_++;
                                }
                            } else stop_flag++;
                            break;
                        } else if (prev_[level]->keys[shift+1] == 0) {
                            prev_[level]->keys[shift+1] = prev_[level-1]->keys[0];
                            prev_[level]->next[shift+1] = prev_[level-1];
                            prev_[level]->N_key++;
                            if (prev_[level]->N_key == ARR_SIZE && prev_[level+1] == head_[level+1]) {
                                level++;
                                if (cur_height < level) {
                                    max_height_++;
                                }
                            } else stop_flag++;
                            break;
                        }
                    }
                }
            } else { // Case 3-2: prev_ node has no room, so we need to make a new node
                // When creating a new node, change prev to the newly created node.
                // TODO:: Need to consider special case that insert into between keys (at node)
                if (level == 0) { // Case 3-2-1: Insert into H0
                    int no_room = 0;
                    if (prev_[level]->keys[ARR_SIZE-1] < key && prev_[level]->keys[ARR_SIZE-1] != 0) {
                        no_room++;
                    } else {
                        for (int shift = 0; shift < prev_[level]->N_key-1; shift++) { // TODO:: Problem is here!!
                            if (prev_[level]->keys[shift] <= key && key < prev_[level]->keys[shift+1]) {
                                Key temp_key = prev_[level]->keys[ARR_SIZE-1];
                                for (int do_shift = prev_[level]->N_key-2; do_shift > shift; do_shift--) {
                                    prev_[level]->keys[do_shift+1] = prev_[level]->keys[do_shift];
                                }
                                prev_[level]->keys[shift+1] = key;
                                //printf("N_key - %d\n", prev_[level]->forward->N_key);
                                if (prev_[level]->forward != nullptr && prev_[level]->forward->N_key < ARR_SIZE) {
                                    Key update_key = prev_[level]->forward->keys[0];
                                    for (int d_shift = prev_[level]->forward->N_key; d_shift > 0; d_shift--) {
                                        prev_[level]->forward->keys[d_shift] = prev_[level]->forward->keys[d_shift-1];
                                    }
                                    prev_[level]->forward->keys[0] = temp_key;
                                    prev_[level]->forward->N_key++;
                                    if (false) {
                                        level++;
                                        if (cur_height < level) {
                                            max_height_++;
                                        } 
                                    } else {
                                        for (int update = level+1; update < GetMaxHeight(); update++) {
                                            int loop = 0;
                                            for (loop = 0; loop < prev_[update]->N_key; loop++) {
                                                if (prev_[update]->keys[loop] == update_key) {
                                                    prev_[update]->keys[loop] = temp_key;
                                                    break;
                                                }
                                            }
                                            if (prev_[update]->forward != nullptr) {
                                                for (loop = 0; loop < prev_[update]->forward->N_key; loop++) {
                                                    if (prev_[update]->forward->keys[loop] == update_key) {
                                                        prev_[update]->forward->keys[loop] = temp_key;
                                                        break;
                                                    }
                                                }
                                            }
                                            if (loop != 0) break;
                                        }
                                        stop_flag++;
                                    }
                                    break;
                                } else {
                                    Node* add_node = NewNode(temp_key);
                                    add_node->forward = prev_[level]->forward;
                                    prev_[level]->forward = add_node;
                                    prev_[level] = add_node;
                                    level++;
                                    if (cur_height < level) {
                                        max_height_++;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (no_room != 0) { // TODO:: Need to modify
                        if (prev_[level]->forward != nullptr && prev_[level]->forward->N_key < ARR_SIZE) {
                            Key update_key = prev_[level]->forward->keys[0];
                            for (int d_shift = prev_[level]->forward->N_key; d_shift > 0; d_shift--) {
                                prev_[level]->forward->keys[d_shift] = prev_[level]->forward->keys[d_shift-1];
                            }
                            prev_[level]->forward->keys[0] = key;
                            prev_[level]->forward->N_key++;
                            if (false) {
                                level++;
                                if (cur_height < level) {
                                    max_height_++;
                                } 
                            } else {
                                for (int update = level+1; update < GetMaxHeight(); update++) {
                                    int loop = 0;
                                    for (loop = 0; loop < prev_[update]->N_key; loop++) {
                                        if (prev_[update]->keys[loop] == update_key) {
                                            prev_[update]->keys[loop] = key;
                                            //prev_[update]->next[loop] = prev_[update-1]->forward;
                                            break;
                                        }
                                    }
                                    if (prev_[update]->forward != nullptr) {
                                        for (loop = 0; loop < prev_[update]->forward->N_key; loop++) {
                                            if (prev_[update]->forward->keys[loop] == update_key) {
                                                prev_[update]->forward->keys[loop] = key;
                                                //prev_[update]->forward->next[loop] = prev_[update-1]->forward;
                                                break;
                                            }
                                        }
                                    }
                                    if (loop != 0) break;
                                }
                                stop_flag++;
                            }
                            break;
                        } else {
                            Node* add_node = NewNode(key);
                            add_node->forward = prev_[level]->forward;
                            prev_[level]->forward = add_node;
                            prev_[level] = add_node;
                            level++;
                            if (cur_height < level) {
                                max_height_++;
                            }
                        }
                    }
                } else { // Case 3-2-2: Not insert into H0
                    int no_room = 0;
                    if (prev_[level]->keys[ARR_SIZE-1] < prev_[level-1]->keys[0] && prev_[level]->keys[ARR_SIZE-1] != 0) {
                        no_room++;
                    } else {
                        for (int shift = 0; shift < prev_[level]->N_key-1; shift++) {
                            if (prev_[level]->keys[shift] <= prev_[level-1]->keys[0] && prev_[level-1]->keys[0] < prev_[level]->keys[shift+1]) {
                                Key temp_key = prev_[level]->keys[ARR_SIZE-1];
                                Node* temp_next = prev_[level]->next[ARR_SIZE-1];
                                for (int do_shift = prev_[level]->N_key-2; do_shift > shift; do_shift--) {
                                    prev_[level]->keys[do_shift+1] = prev_[level]->keys[do_shift];
                                    prev_[level]->next[do_shift+1] = prev_[level]->next[do_shift];
                                }
                                prev_[level]->keys[shift+1] = prev_[level-1]->keys[0];
                                prev_[level]->next[shift+1] = prev_[level-1];
                                if (prev_[level]->forward != nullptr && prev_[level]->forward->N_key < ARR_SIZE) {
                                    Key update_key = prev_[level]->forward->keys[0];
                                    for (int d_shift = prev_[level]->forward->N_key; d_shift > 0; d_shift--) {
                                        prev_[level]->forward->keys[d_shift] = prev_[level]->forward->keys[d_shift-1];
                                        prev_[level]->forward->next[d_shift] = prev_[level]->forward->next[d_shift-1];
                                    }
                                    prev_[level]->forward->keys[0] = temp_key;
                                    prev_[level]->forward->next[0] = temp_next;
                                    prev_[level]->forward->N_key++;
                                    if (false) {
                                        level++;
                                        if (cur_height < level) {
                                            max_height_++;
                                        } 
                                    } else {
                                        for (int update = level+1; update < GetMaxHeight(); update++) {
                                            int loop = 0;
                                            for (loop = 0; loop < prev_[update]->N_key; loop++) {
                                                if (prev_[update]->keys[loop] == update_key) {
                                                    prev_[update]->keys[loop] = temp_key;
                                                    break;
                                                }
                                            }
                                            if (prev_[update]->forward != nullptr) {
                                                for (loop = 0; loop < prev_[update]->forward->N_key; loop++) {
                                                    if (prev_[update]->forward->keys[loop] == update_key) {
                                                        prev_[update]->forward->keys[loop] = temp_key;
                                                        break;
                                                    }
                                                }
                                            }
                                            if (loop != 0) break;
                                        }
                                        stop_flag++;
                                    }
                                    break;
                                } else {
                                    Node* add_node = NewNode(temp_key);
                                    add_node->forward = prev_[level]->forward;
                                    add_node->next[0] = temp_next;
                                    prev_[level]->forward = add_node;
                                    prev_[level] = add_node;
                                    level++;
                                    if (cur_height < level) {
                                        max_height_++;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (no_room != 0) {
                        if (prev_[level]->forward != nullptr && prev_[level]->forward->N_key < ARR_SIZE) {
                            Key update_key = prev_[level]->forward->keys[0];
                            for (int d_shift = prev_[level]->forward->N_key; d_shift > 0; d_shift--) {
                                prev_[level]->forward->keys[d_shift] = prev_[level]->forward->keys[d_shift-1];
                                prev_[level]->forward->next[d_shift] = prev_[level]->forward->next[d_shift-1];
                            }
                            prev_[level]->forward->keys[0] = prev_[level-1]->keys[0];
                            prev_[level]->forward->next[0] = prev_[level-1];
                            prev_[level]->forward->N_key++;
                            if (false) {
                                level++;
                                if (cur_height < level) {
                                    max_height_++;
                                } 
                            } else {
                                for (int update = level+1; update < GetMaxHeight(); update++) {
                                    int loop = 0;
                                    for (loop = 0; loop < prev_[update]->N_key; loop++) {
                                        if (prev_[update]->keys[loop] == update_key) {
                                            prev_[update]->keys[loop] = prev_[level]->forward->keys[0];
                                            break;
                                        }
                                    }
                                    if (prev_[update]->forward != nullptr) {
                                        for (loop = 0; loop < prev_[update]->forward->N_key; loop++) {
                                            if (prev_[update]->forward->keys[loop] == update_key) {
                                                prev_[update]->forward->keys[loop] = prev_[level]->forward->keys[0];
                                                break;
                                            }
                                        }
                                    }
                                    if (loop != 0) break;
                                }
                                stop_flag++;
                            }
                            break;
                        } else {
                            Node* add_node = NewNode(prev_[level-1]->keys[0]);
                            add_node->forward = prev_[level]->forward;
                            add_node->next[0] = prev_[level-1];
                            prev_[level]->forward = add_node;
                            prev_[level] = add_node;
                            level++;
                            if (cur_height < level) {
                                max_height_++;
                            }
                        }
                    }    
                }
            }
        }

        if (stop_flag > 0) {
            break;
        }
    }
}

template<typename Key>
void SkipList<Key>::Print() const {
    printf("Print height - %d\n", GetMaxHeight());
    for(int i = 0; i < GetMaxHeight(); i++) {
        if (head_[i]->forward != nullptr) {
            Node* x = head_[i];
            while (x->forward != nullptr) {
                x = x->forward;
                for (int i = 0; i < x->N_key; i++) {
                    std::cout << x->keys[i] << " ";
                }
                std::cout << std::endl;
            }
        }
        printf("Next\n");
    }
    
}

template<typename Key>
bool SkipList<Key>::Contains(const Key& key) const {
    int height = GetMaxHeight() - 1;
    int pivot = static_cast<int>(log2(ARR_SIZE));
    int loop = ARR_SIZE/2;
    int result_key = -1;

    Node* x = head_[height]; // Use when searching
    while (true) { // Find the location which will insert the key using prev_ and head_
        while (x->forward != nullptr && compare_(x->forward->keys[0], key) <= 0) {
            x = x->forward;
        }
        height--;
        if (height >= 0) {
            if (x == head_[height+1]) {
                x = head_[height];
            } else {
                for (int i = 0; i < x->N_key; i++) {
                    if (x->keys[i] <= key && key < x->keys[i+1]) {
                        x = x->next[i];
                        break;
                    } else if (x->keys[i+1] == 0) {
                        x = x->next[i];
                        break;
                    } else if (x->keys[ARR_SIZE-1] <= key && x->keys[ARR_SIZE-1] != 0) {
                        x = x->next[ARR_SIZE-1];
                        break;
                    }
                }
            }
        } else break;
    }
    // Find key from entry(Array)
    if (compare_(x->keys[0], key) == 0) {
        result_key = x->keys[0];
    } else {
        for(int i = 0; i < pivot; i++) {
            if (compare_(x->keys[loop], key) == 0) {
                result_key = x->keys[loop];
                break;
            } else if (compare_(x->keys[loop], 0) == 0) {
                loop = loop - ARR_SIZE / (1 << (i+2));
            } else if (compare_(x->keys[loop], key) < 0) {
                loop = loop + ARR_SIZE / (1 << (i+2));
            } else if (compare_(x->keys[loop], key) > 0) {
                loop = loop - ARR_SIZE / (1 << (i+2));
            }
        }
    }

    //printf("rk = %d, k = %d\n", result_key, key);

    if (x != nullptr && compare_(result_key, key) == 0) {
        return true;
    } else {
        //printf("False\n");
        return false;
    }
}

template<typename Key>
void SkipList<Key>::Scan(const Key& key, const int scan_num) {
    int height = GetMaxHeight() - 1;
    int pivot = static_cast<int>(log2(ARR_SIZE));
    int loop = ARR_SIZE/2;

    Key* scan_buf = new Key[scan_num];

    Node* x = head_[height]; // Use when searching
    while (true) { // Find the location which will insert the key using prev_ and head_
        while (x->forward != nullptr && compare_(x->forward->keys[0], key) <= 0) {
            x = x->forward;
        }
        height--;
        if (height >= 0) {
            if (x == head_[height+1]) {
                x = head_[height];
            } else {
                for (int i = 0; i < x->N_key; i++) {
                    if (x->keys[i] <= key && key < x->keys[i+1]) {
                        x = x->next[i];
                        break;
                    } else if (x->keys[i+1] == 0) {
                        x = x->next[i];
                        break;
                    } else if (x->keys[ARR_SIZE-1] <= key && x->keys[ARR_SIZE-1] != 0) {
                        x = x->next[ARR_SIZE-1];
                        break;
                    }
                }
            }
        } else break;
    }
    // Find key from entry(Array)
    if (compare_(x->keys[0], key) == 0) {
        loop = 0;
    } else {
        for(int i = 0; i < pivot; i++) {
            if (compare_(x->keys[loop], key) == 0) {
                break;
            } else if (compare_(x->keys[loop], 0) == 0) {
                loop = loop - ARR_SIZE / (1 << (i+2));
            } else if (compare_(x->keys[loop], key) < 0) {
                loop = loop + ARR_SIZE / (1 << (i+2));
            } else if (compare_(x->keys[loop], key) > 0) {
                loop = loop - ARR_SIZE / (1 << (i+2));
            }
        }
    }
    int i = 0;
    while (true) {
        if (loop != 0) {
            for (int j = loop; j < ARR_SIZE; j++) {
                scan_buf[i] = x->keys[j];
                i++;
            }
        } else {
            for (int k = 0; k < ARR_SIZE; k++) {
                scan_buf[i] = x->keys[k];
                i++;
            }
        }
        if (i >= scan_num-1) {
            break;
        }
        if (x->forward != nullptr) {
            x = x->forward;
        } else {
            break;;
        }
    }
}