if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/optimistic_transaction_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/optimistic_transaction_test[1]_tests.cmake")
else()
  add_test(optimistic_transaction_test_NOT_BUILT optimistic_transaction_test_NOT_BUILT)
endif()
