if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/sst_file_reader_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/sst_file_reader_test[1]_tests.cmake")
else()
  add_test(sst_file_reader_test_NOT_BUILT sst_file_reader_test_NOT_BUILT)
endif()
