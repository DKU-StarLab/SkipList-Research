if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/db_logical_block_size_cache_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/db_logical_block_size_cache_test[1]_tests.cmake")
else()
  add_test(db_logical_block_size_cache_test_NOT_BUILT db_logical_block_size_cache_test_NOT_BUILT)
endif()
