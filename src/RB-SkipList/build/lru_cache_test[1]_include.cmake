if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/lru_cache_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/lru_cache_test[1]_tests.cmake")
else()
  add_test(lru_cache_test_NOT_BUILT lru_cache_test_NOT_BUILT)
endif()
