if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/import_column_family_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/import_column_family_test[1]_tests.cmake")
else()
  add_test(import_column_family_test_NOT_BUILT import_column_family_test_NOT_BUILT)
endif()
