# SkipList-Research

SkipList-Research provides code and an experiment environment that analyzes skip lists and implements various optimization methods.

You can check the research topics in progress in the items below.

## Contents

- **Compact-SkipList**
  - CSL (Compact SkipList) is a structure that has a skip list in the upper area and an array in the lower area. When a new key is inserted, modular operation is performed. The quotient obtained through modular operation is stored as a skip list node in the upper area, and sorting in the upper area is guaranteed based on this. In the lower area of the array, the remainder obtained through modular operation points to the index of the array, and the actual key value is stored at that index. 

- **RB-SkipList**
  - RB-SkipList (Red Black SkipList) has an RB-Tree structure in the upper area and a skip list structure in the lower area. In the upper area, RB-Tree, only the nodes at the highest height of the skip list are managed. Conventionally, insertion and lookup operations start from the head node at the highest height, but RB-SkipList searches from the root node of the tree, which is the upper area. Quickly access the appropriate location in the skip list through the tree structure and then proceed with the search in the same way as the existing skip list.

- **Arrayed SkipList**
  - ASL is a modified skip list structure consisting of multiple linked lists. Existing linked list nodes consist of a single node, but instead of a single node, multiple nodes are combined into an array. Construct a skip list using a list consisting of an array. The array size used in this study was 4.

**CSL** and **RB-SkipList** modified the skip list code used in **RocksDB**.

Detailed information (functions, installation) can be found in src.
