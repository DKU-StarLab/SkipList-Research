if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/stats_history_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/stats_history_test[1]_tests.cmake")
else()
  add_test(stats_history_test_NOT_BUILT stats_history_test_NOT_BUILT)
endif()
