if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/block_fetcher_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/block_fetcher_test[1]_tests.cmake")
else()
  add_test(block_fetcher_test_NOT_BUILT block_fetcher_test_NOT_BUILT)
endif()
