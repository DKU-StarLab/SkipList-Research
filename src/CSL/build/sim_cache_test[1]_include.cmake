if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/sim_cache_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/sim_cache_test[1]_tests.cmake")
else()
  add_test(sim_cache_test_NOT_BUILT sim_cache_test_NOT_BUILT)
endif()
