if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/range_tombstone_fragmenter_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/range_tombstone_fragmenter_test[1]_tests.cmake")
else()
  add_test(range_tombstone_fragmenter_test_NOT_BUILT range_tombstone_fragmenter_test_NOT_BUILT)
endif()
