if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/write_callback_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/write_callback_test[1]_tests.cmake")
else()
  add_test(write_callback_test_NOT_BUILT write_callback_test_NOT_BUILT)
endif()
