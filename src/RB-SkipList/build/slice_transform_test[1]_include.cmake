if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/slice_transform_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/slice_transform_test[1]_tests.cmake")
else()
  add_test(slice_transform_test_NOT_BUILT slice_transform_test_NOT_BUILT)
endif()
