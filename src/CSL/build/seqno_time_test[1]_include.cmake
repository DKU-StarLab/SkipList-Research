if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/seqno_time_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/seqno_time_test[1]_tests.cmake")
else()
  add_test(seqno_time_test_NOT_BUILT seqno_time_test_NOT_BUILT)
endif()
