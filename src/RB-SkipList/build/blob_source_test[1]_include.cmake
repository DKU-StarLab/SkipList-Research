if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/blob_source_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/blob_source_test[1]_tests.cmake")
else()
  add_test(blob_source_test_NOT_BUILT blob_source_test_NOT_BUILT)
endif()
