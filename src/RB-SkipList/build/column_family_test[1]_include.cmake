if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/column_family_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/column_family_test[1]_tests.cmake")
else()
  add_test(column_family_test_NOT_BUILT column_family_test_NOT_BUILT)
endif()
