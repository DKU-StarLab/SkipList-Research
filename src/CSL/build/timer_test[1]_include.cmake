if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/timer_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/timer_test[1]_tests.cmake")
else()
  add_test(timer_test_NOT_BUILT timer_test_NOT_BUILT)
endif()
