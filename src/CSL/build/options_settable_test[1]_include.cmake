if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/options_settable_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/options_settable_test[1]_tests.cmake")
else()
  add_test(options_settable_test_NOT_BUILT options_settable_test_NOT_BUILT)
endif()
