# Compact SkipList

## Compact-SkipList Functions

```C++

  void Insert_Buf(const Key& key); // Insert key into Compact-SkipList

  bool Contains_Buf(const Key& key) const; // Lookup key from Compact-SkipList

  vool Contains_Sean(const Key& key, uint64_t scan_num) const; // Scan key from Compact-SkipList

```

## Compact-SkipList workload

When using the workload, you must uncomment it and proceed.

- SeqInsertAndLookupX : Sequential pattern and lookup non-existent key

- SeqInsertAndLookupO : Sequential pattern and lookup existent key

- UniRandInsertAndLookup : Uniform pattern

- ZipRandInsertAndLookup : Zipfian pattern

- YCSBWorkloadA~F : YCSB workload A to F

### All codes are continuously being updated.

## RocksDB: A Persistent Key-Value Store for Flash and RAM Storage

[![CircleCI Status](https://circleci.com/gh/facebook/rocksdb.svg?style=svg)](https://circleci.com/gh/facebook/rocksdb)

RocksDB is developed and maintained by Facebook Database Engineering Team.
It is built on earlier work on [LevelDB](https://github.com/google/leveldb) by Sanjay Ghemawat (sanjay@google.com)
and Jeff Dean (jeff@google.com)

This code is a library that forms the core building block for a fast
key-value server, especially suited for storing data on flash drives.
It has a Log-Structured-Merge-Database (LSM) design with flexible tradeoffs
between Write-Amplification-Factor (WAF), Read-Amplification-Factor (RAF)
and Space-Amplification-Factor (SAF). It has multi-threaded compactions,
making it especially suitable for storing multiple terabytes of data in a
single database.

Start with example usage here: https://github.com/facebook/rocksdb/tree/main/examples

See the [github wiki](https://github.com/facebook/rocksdb/wiki) for more explanation.

The public interface is in `include/`.  Callers should not include or
rely on the details of any other header files in this package.  Those
internal APIs may be changed without warning.

Questions and discussions are welcome on the [RocksDB Developers Public](https://www.facebook.com/groups/rocksdb.dev/) Facebook group and [email list](https://groups.google.com/g/rocksdb) on Google Groups.

## License

RocksDB is dual-licensed under both the GPLv2 (found in the COPYING file in the root directory) and Apache 2.0 License (found in the LICENSE.Apache file in the root directory).  You may select, at your option, one of the above-listed licenses.
