if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/periodic_task_scheduler_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/periodic_task_scheduler_test[1]_tests.cmake")
else()
  add_test(periodic_task_scheduler_test_NOT_BUILT periodic_task_scheduler_test_NOT_BUILT)
endif()
