if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/random_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/random_test[1]_tests.cmake")
else()
  add_test(random_test_NOT_BUILT random_test_NOT_BUILT)
endif()
