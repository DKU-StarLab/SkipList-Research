if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/tiered_compaction_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/tiered_compaction_test[1]_tests.cmake")
else()
  add_test(tiered_compaction_test_NOT_BUILT tiered_compaction_test_NOT_BUILT)
endif()
