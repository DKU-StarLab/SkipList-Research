if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/coding_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/coding_test[1]_tests.cmake")
else()
  add_test(coding_test_NOT_BUILT coding_test_NOT_BUILT)
endif()
