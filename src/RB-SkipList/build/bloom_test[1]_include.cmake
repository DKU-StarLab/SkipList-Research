if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/bloom_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/bloom_test[1]_tests.cmake")
else()
  add_test(bloom_test_NOT_BUILT bloom_test_NOT_BUILT)
endif()
