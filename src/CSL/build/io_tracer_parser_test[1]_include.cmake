if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/io_tracer_parser_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/io_tracer_parser_test[1]_tests.cmake")
else()
  add_test(io_tracer_parser_test_NOT_BUILT io_tracer_parser_test_NOT_BUILT)
endif()
