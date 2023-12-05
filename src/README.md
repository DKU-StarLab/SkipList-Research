# Installation and Usage

CSL and RB-SkipList may require modifications in the code before proceeding with the experiment.

## Common Parameter

- In skiplist.h

    - int32_t **max_height** : Set the maximum height of the skip list

- In skiplist_test.cc

    - const int **N** : Number of write operations

    - const int **R** : Number of read operations


Detailed information regarding settings is explained within each directory.

- Compact SkipList

```
    make skiplist_test

    ./skiplist_test
```

- RB-SkipList

```
    make skiplist_test

    ./skiplist_test
```

If you want to run make quickly, you can use the **-j** option.

Usage: make -j{nproc} skiplist_test / nproc = number of CPU cores
