if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/histogram_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/histogram_test[1]_tests.cmake")
else()
  add_test(histogram_test_NOT_BUILT histogram_test_NOT_BUILT)
endif()
