# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aryan011/compiler/hydrogen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aryan011/compiler/hydrogen/cmake-build-debug-wsl

# Include any dependencies generated for this target.
include CMakeFiles/hydro.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hydro.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hydro.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hydro.dir/flags.make

CMakeFiles/hydro.dir/src/main.cpp.o: CMakeFiles/hydro.dir/flags.make
CMakeFiles/hydro.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/hydro.dir/src/main.cpp.o: CMakeFiles/hydro.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aryan011/compiler/hydrogen/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hydro.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hydro.dir/src/main.cpp.o -MF CMakeFiles/hydro.dir/src/main.cpp.o.d -o CMakeFiles/hydro.dir/src/main.cpp.o -c /home/aryan011/compiler/hydrogen/src/main.cpp

CMakeFiles/hydro.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hydro.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aryan011/compiler/hydrogen/src/main.cpp > CMakeFiles/hydro.dir/src/main.cpp.i

CMakeFiles/hydro.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hydro.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aryan011/compiler/hydrogen/src/main.cpp -o CMakeFiles/hydro.dir/src/main.cpp.s

CMakeFiles/hydro.dir/src/token.cpp.o: CMakeFiles/hydro.dir/flags.make
CMakeFiles/hydro.dir/src/token.cpp.o: ../src/token.cpp
CMakeFiles/hydro.dir/src/token.cpp.o: CMakeFiles/hydro.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aryan011/compiler/hydrogen/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hydro.dir/src/token.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hydro.dir/src/token.cpp.o -MF CMakeFiles/hydro.dir/src/token.cpp.o.d -o CMakeFiles/hydro.dir/src/token.cpp.o -c /home/aryan011/compiler/hydrogen/src/token.cpp

CMakeFiles/hydro.dir/src/token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hydro.dir/src/token.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aryan011/compiler/hydrogen/src/token.cpp > CMakeFiles/hydro.dir/src/token.cpp.i

CMakeFiles/hydro.dir/src/token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hydro.dir/src/token.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aryan011/compiler/hydrogen/src/token.cpp -o CMakeFiles/hydro.dir/src/token.cpp.s

# Object files for target hydro
hydro_OBJECTS = \
"CMakeFiles/hydro.dir/src/main.cpp.o" \
"CMakeFiles/hydro.dir/src/token.cpp.o"

# External object files for target hydro
hydro_EXTERNAL_OBJECTS =

hydro: CMakeFiles/hydro.dir/src/main.cpp.o
hydro: CMakeFiles/hydro.dir/src/token.cpp.o
hydro: CMakeFiles/hydro.dir/build.make
hydro: CMakeFiles/hydro.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aryan011/compiler/hydrogen/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable hydro"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hydro.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hydro.dir/build: hydro
.PHONY : CMakeFiles/hydro.dir/build

CMakeFiles/hydro.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hydro.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hydro.dir/clean

CMakeFiles/hydro.dir/depend:
	cd /home/aryan011/compiler/hydrogen/cmake-build-debug-wsl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aryan011/compiler/hydrogen /home/aryan011/compiler/hydrogen /home/aryan011/compiler/hydrogen/cmake-build-debug-wsl /home/aryan011/compiler/hydrogen/cmake-build-debug-wsl /home/aryan011/compiler/hydrogen/cmake-build-debug-wsl/CMakeFiles/hydro.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hydro.dir/depend

