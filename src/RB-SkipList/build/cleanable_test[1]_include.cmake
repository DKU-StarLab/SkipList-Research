if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/cleanable_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/cleanable_test[1]_tests.cmake")
else()
  add_test(cleanable_test_NOT_BUILT cleanable_test_NOT_BUILT)
endif()
