if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/memory_allocator_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/memory_allocator_test[1]_tests.cmake")
else()
  add_test(memory_allocator_test_NOT_BUILT memory_allocator_test_NOT_BUILT)
endif()