if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/dbformat_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/dbformat_test[1]_tests.cmake")
else()
  add_test(dbformat_test_NOT_BUILT dbformat_test_NOT_BUILT)
endif()
