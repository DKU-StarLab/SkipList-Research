if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/io_posix_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/io_posix_test[1]_tests.cmake")
else()
  add_test(io_posix_test_NOT_BUILT io_posix_test_NOT_BUILT)
endif()
