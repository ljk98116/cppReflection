# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/lujuankui/cmake-install/bin/cmake

# The command to remove a file.
RM = /home/lujuankui/cmake-install/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/personal_projects/Reflection

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/personal_projects/Reflection/build

# Include any dependencies generated for this target.
include test/CMakeFiles/PropertyTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/PropertyTest.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/PropertyTest.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/PropertyTest.dir/flags.make

test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.o: test/CMakeFiles/PropertyTest.dir/flags.make
test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.o: ../test/PropertyTest.cc
test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.o: test/CMakeFiles/PropertyTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.o"
	cd /data/personal_projects/Reflection/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.o -MF CMakeFiles/PropertyTest.dir/PropertyTest.cc.o.d -o CMakeFiles/PropertyTest.dir/PropertyTest.cc.o -c /data/personal_projects/Reflection/test/PropertyTest.cc

test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PropertyTest.dir/PropertyTest.cc.i"
	cd /data/personal_projects/Reflection/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/personal_projects/Reflection/test/PropertyTest.cc > CMakeFiles/PropertyTest.dir/PropertyTest.cc.i

test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PropertyTest.dir/PropertyTest.cc.s"
	cd /data/personal_projects/Reflection/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/personal_projects/Reflection/test/PropertyTest.cc -o CMakeFiles/PropertyTest.dir/PropertyTest.cc.s

# Object files for target PropertyTest
PropertyTest_OBJECTS = \
"CMakeFiles/PropertyTest.dir/PropertyTest.cc.o"

# External object files for target PropertyTest
PropertyTest_EXTERNAL_OBJECTS =

test/PropertyTest: test/CMakeFiles/PropertyTest.dir/PropertyTest.cc.o
test/PropertyTest: test/CMakeFiles/PropertyTest.dir/build.make
test/PropertyTest: src/libreflection.so
test/PropertyTest: lib/libgtest_main.a
test/PropertyTest: lib/libgtest.a
test/PropertyTest: test/CMakeFiles/PropertyTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PropertyTest"
	cd /data/personal_projects/Reflection/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PropertyTest.dir/link.txt --verbose=$(VERBOSE)
	cd /data/personal_projects/Reflection/build/test && /home/lujuankui/cmake-install/bin/cmake -D TEST_TARGET=PropertyTest -D TEST_EXECUTABLE=/data/personal_projects/Reflection/build/test/PropertyTest -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/data/personal_projects/Reflection/build/test -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=PropertyTest_TESTS -D CTEST_FILE=/data/personal_projects/Reflection/build/test/PropertyTest[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /home/lujuankui/cmake-install/share/cmake-3.21/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
test/CMakeFiles/PropertyTest.dir/build: test/PropertyTest
.PHONY : test/CMakeFiles/PropertyTest.dir/build

test/CMakeFiles/PropertyTest.dir/clean:
	cd /data/personal_projects/Reflection/build/test && $(CMAKE_COMMAND) -P CMakeFiles/PropertyTest.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/PropertyTest.dir/clean

test/CMakeFiles/PropertyTest.dir/depend:
	cd /data/personal_projects/Reflection/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/personal_projects/Reflection /data/personal_projects/Reflection/test /data/personal_projects/Reflection/build /data/personal_projects/Reflection/build/test /data/personal_projects/Reflection/build/test/CMakeFiles/PropertyTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/PropertyTest.dir/depend

