# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shin96/workspace_starlab/co-workspace/Compact-SkipList

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build

# Include any dependencies generated for this target.
include CMakeFiles/io_tracer_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/io_tracer_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/io_tracer_test.dir/flags.make

CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.o: CMakeFiles/io_tracer_test.dir/flags.make
CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.o: ../trace_replay/io_tracer_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.o -c /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/trace_replay/io_tracer_test.cc

CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/trace_replay/io_tracer_test.cc > CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.i

CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/trace_replay/io_tracer_test.cc -o CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.s

# Object files for target io_tracer_test
io_tracer_test_OBJECTS = \
"CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.o"

# External object files for target io_tracer_test
io_tracer_test_EXTERNAL_OBJECTS =

io_tracer_test: CMakeFiles/io_tracer_test.dir/trace_replay/io_tracer_test.cc.o
io_tracer_test: CMakeFiles/io_tracer_test.dir/build.make
io_tracer_test: libtestutillib.a
io_tracer_test: libtestharness.a
io_tracer_test: third-party/gtest-1.8.1/fused-src/gtest/libgtest.a
io_tracer_test: librocksdb.so.8.3.0
io_tracer_test: /usr/lib/x86_64-linux-gnu/libgflags.so.2.2.2
io_tracer_test: CMakeFiles/io_tracer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable io_tracer_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/io_tracer_test.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=io_tracer_test -D TEST_EXECUTABLE=/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/io_tracer_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=io_tracer_test_TESTS -D CTEST_FILE=/home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/io_tracer_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=120 -P /usr/share/cmake-3.16/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/io_tracer_test.dir/build: io_tracer_test

.PHONY : CMakeFiles/io_tracer_test.dir/build

CMakeFiles/io_tracer_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/io_tracer_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/io_tracer_test.dir/clean

CMakeFiles/io_tracer_test.dir/depend:
	cd /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shin96/workspace_starlab/co-workspace/Compact-SkipList /home/shin96/workspace_starlab/co-workspace/Compact-SkipList /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build /home/shin96/workspace_starlab/co-workspace/Compact-SkipList/build/CMakeFiles/io_tracer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/io_tracer_test.dir/depend

