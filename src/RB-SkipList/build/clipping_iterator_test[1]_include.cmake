if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/clipping_iterator_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/clipping_iterator_test[1]_tests.cmake")
else()
  add_test(clipping_iterator_test_NOT_BUILT clipping_iterator_test_NOT_BUILT)
endif()
