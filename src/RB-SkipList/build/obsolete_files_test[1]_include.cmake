if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/obsolete_files_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/obsolete_files_test[1]_tests.cmake")
else()
  add_test(obsolete_files_test_NOT_BUILT obsolete_files_test_NOT_BUILT)
endif()
