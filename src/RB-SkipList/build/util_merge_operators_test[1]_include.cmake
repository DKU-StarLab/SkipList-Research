if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/util_merge_operators_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/util_merge_operators_test[1]_tests.cmake")
else()
  add_test(util_merge_operators_test_NOT_BUILT util_merge_operators_test_NOT_BUILT)
endif()