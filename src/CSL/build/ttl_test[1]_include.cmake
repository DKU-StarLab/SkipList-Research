if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/ttl_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/ttl_test[1]_tests.cmake")
else()
  add_test(ttl_test_NOT_BUILT ttl_test_NOT_BUILT)
endif()
