if(EXISTS "/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/cassandra_format_test[1]_tests.cmake")
  include("/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/cassandra_format_test[1]_tests.cmake")
else()
  add_test(cassandra_format_test_NOT_BUILT cassandra_format_test_NOT_BUILT)
endif()
