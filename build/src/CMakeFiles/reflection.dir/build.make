# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /data/cmake-3.24.4/bin/cmake

# The command to remove a file.
RM = /data/cmake-3.24.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/personal_projects/Reflection

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/personal_projects/Reflection/build

# Include any dependencies generated for this target.
include src/CMakeFiles/reflection.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/reflection.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/reflection.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/reflection.dir/flags.make

src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o: src/CMakeFiles/reflection.dir/flags.make
src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o: /data/personal_projects/Reflection/src/Interface/IReflectionable.cpp
src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o: src/CMakeFiles/reflection.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o -MF CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o.d -o CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o -c /data/personal_projects/Reflection/src/Interface/IReflectionable.cpp

src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.i"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/personal_projects/Reflection/src/Interface/IReflectionable.cpp > CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.i

src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.s"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/personal_projects/Reflection/src/Interface/IReflectionable.cpp -o CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.s

src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o: src/CMakeFiles/reflection.dir/flags.make
src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o: /data/personal_projects/Reflection/src/Type/MemberInfo.cpp
src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o: src/CMakeFiles/reflection.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o -MF CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o.d -o CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o -c /data/personal_projects/Reflection/src/Type/MemberInfo.cpp

src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reflection.dir/Type/MemberInfo.cpp.i"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/personal_projects/Reflection/src/Type/MemberInfo.cpp > CMakeFiles/reflection.dir/Type/MemberInfo.cpp.i

src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reflection.dir/Type/MemberInfo.cpp.s"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/personal_projects/Reflection/src/Type/MemberInfo.cpp -o CMakeFiles/reflection.dir/Type/MemberInfo.cpp.s

src/CMakeFiles/reflection.dir/Type/Object.cpp.o: src/CMakeFiles/reflection.dir/flags.make
src/CMakeFiles/reflection.dir/Type/Object.cpp.o: /data/personal_projects/Reflection/src/Type/Object.cpp
src/CMakeFiles/reflection.dir/Type/Object.cpp.o: src/CMakeFiles/reflection.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/reflection.dir/Type/Object.cpp.o"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/reflection.dir/Type/Object.cpp.o -MF CMakeFiles/reflection.dir/Type/Object.cpp.o.d -o CMakeFiles/reflection.dir/Type/Object.cpp.o -c /data/personal_projects/Reflection/src/Type/Object.cpp

src/CMakeFiles/reflection.dir/Type/Object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reflection.dir/Type/Object.cpp.i"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/personal_projects/Reflection/src/Type/Object.cpp > CMakeFiles/reflection.dir/Type/Object.cpp.i

src/CMakeFiles/reflection.dir/Type/Object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reflection.dir/Type/Object.cpp.s"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/personal_projects/Reflection/src/Type/Object.cpp -o CMakeFiles/reflection.dir/Type/Object.cpp.s

src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o: src/CMakeFiles/reflection.dir/flags.make
src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o: /data/personal_projects/Reflection/src/Type/TypeFactory.cpp
src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o: src/CMakeFiles/reflection.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o -MF CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o.d -o CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o -c /data/personal_projects/Reflection/src/Type/TypeFactory.cpp

src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reflection.dir/Type/TypeFactory.cpp.i"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/personal_projects/Reflection/src/Type/TypeFactory.cpp > CMakeFiles/reflection.dir/Type/TypeFactory.cpp.i

src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reflection.dir/Type/TypeFactory.cpp.s"
	cd /data/personal_projects/Reflection/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/personal_projects/Reflection/src/Type/TypeFactory.cpp -o CMakeFiles/reflection.dir/Type/TypeFactory.cpp.s

# Object files for target reflection
reflection_OBJECTS = \
"CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o" \
"CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o" \
"CMakeFiles/reflection.dir/Type/Object.cpp.o" \
"CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o"

# External object files for target reflection
reflection_EXTERNAL_OBJECTS =

src/libreflection.so: src/CMakeFiles/reflection.dir/Interface/IReflectionable.cpp.o
src/libreflection.so: src/CMakeFiles/reflection.dir/Type/MemberInfo.cpp.o
src/libreflection.so: src/CMakeFiles/reflection.dir/Type/Object.cpp.o
src/libreflection.so: src/CMakeFiles/reflection.dir/Type/TypeFactory.cpp.o
src/libreflection.so: src/CMakeFiles/reflection.dir/build.make
src/libreflection.so: src/CMakeFiles/reflection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/personal_projects/Reflection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library libreflection.so"
	cd /data/personal_projects/Reflection/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reflection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/reflection.dir/build: src/libreflection.so
.PHONY : src/CMakeFiles/reflection.dir/build

src/CMakeFiles/reflection.dir/clean:
	cd /data/personal_projects/Reflection/build/src && $(CMAKE_COMMAND) -P CMakeFiles/reflection.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/reflection.dir/clean

src/CMakeFiles/reflection.dir/depend:
	cd /data/personal_projects/Reflection/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/personal_projects/Reflection /data/personal_projects/Reflection/src /data/personal_projects/Reflection/build /data/personal_projects/Reflection/build/src /data/personal_projects/Reflection/build/src/CMakeFiles/reflection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/reflection.dir/depend

