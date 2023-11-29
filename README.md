# SkipList-Research

SkipList-Research provides code and an experiment environment that analyzes skip lists and implements various optimization methods.

You can check the research topics in progress in the items below.

## Contents

- **Compact-SkipList**
  - CSL is a structure that has a skip list in the upper area and an array in the lower area. When a new key is inserted, modular operation is performed. The quotient obtained through modular operation is stored as a skip list node in the upper area, and sorting in the upper area is guaranteed based on this. In the lower area of the array, the remainder obtained through modular operation points to the index of the array, and the actual key value is stored at that index. 

- **RB-SkipList**
  - 

- **Arrayed SkipList**
  - 

Detailed information can be found in src.
