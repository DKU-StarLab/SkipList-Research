if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/cuckoo_table_reader_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/cuckoo_table_reader_test[1]_tests.cmake")
else()
  add_test(cuckoo_table_reader_test_NOT_BUILT cuckoo_table_reader_test_NOT_BUILT)
endif()
