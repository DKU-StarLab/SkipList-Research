if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/db_tailing_iter_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/db_tailing_iter_test[1]_tests.cmake")
else()
  add_test(db_tailing_iter_test_NOT_BUILT db_tailing_iter_test_NOT_BUILT)
endif()
