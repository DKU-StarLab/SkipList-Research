if(EXISTS "/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/memory_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/LA-SkipList/build/memory_test[1]_tests.cmake")
else()
  add_test(memory_test_NOT_BUILT memory_test_NOT_BUILT)
endif()