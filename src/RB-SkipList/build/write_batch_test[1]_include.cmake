if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/write_batch_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/write_batch_test[1]_tests.cmake")
else()
  add_test(write_batch_test_NOT_BUILT write_batch_test_NOT_BUILT)
endif()
