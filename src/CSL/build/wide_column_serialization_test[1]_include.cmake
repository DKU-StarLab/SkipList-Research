if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/wide_column_serialization_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/wide_column_serialization_test[1]_tests.cmake")
else()
  add_test(wide_column_serialization_test_NOT_BUILT wide_column_serialization_test_NOT_BUILT)
endif()