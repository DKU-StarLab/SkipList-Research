if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/cache_reservation_manager_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/cache_reservation_manager_test[1]_tests.cmake")
else()
  add_test(cache_reservation_manager_test_NOT_BUILT cache_reservation_manager_test_NOT_BUILT)
endif()
