if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/persistent_cache_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/persistent_cache_test[1]_tests.cmake")
else()
  add_test(persistent_cache_test_NOT_BUILT persistent_cache_test_NOT_BUILT)
endif()