# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /opt/clion-2019.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tester/CLionProjects/DED/Onegin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tester/CLionProjects/DED/Onegin/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Onegin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Onegin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Onegin.dir/flags.make

CMakeFiles/Onegin.dir/main.cpp.o: CMakeFiles/Onegin.dir/flags.make
CMakeFiles/Onegin.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/CLionProjects/DED/Onegin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Onegin.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Onegin.dir/main.cpp.o -c /home/tester/CLionProjects/DED/Onegin/main.cpp

CMakeFiles/Onegin.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Onegin.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/CLionProjects/DED/Onegin/main.cpp > CMakeFiles/Onegin.dir/main.cpp.i

CMakeFiles/Onegin.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Onegin.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/CLionProjects/DED/Onegin/main.cpp -o CMakeFiles/Onegin.dir/main.cpp.s

# Object files for target Onegin
Onegin_OBJECTS = \
"CMakeFiles/Onegin.dir/main.cpp.o"

# External object files for target Onegin
Onegin_EXTERNAL_OBJECTS =

Onegin: CMakeFiles/Onegin.dir/main.cpp.o
Onegin: CMakeFiles/Onegin.dir/build.make
Onegin: CMakeFiles/Onegin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tester/CLionProjects/DED/Onegin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Onegin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Onegin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Onegin.dir/build: Onegin

.PHONY : CMakeFiles/Onegin.dir/build

CMakeFiles/Onegin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Onegin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Onegin.dir/clean

CMakeFiles/Onegin.dir/depend:
	cd /home/tester/CLionProjects/DED/Onegin/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tester/CLionProjects/DED/Onegin /home/tester/CLionProjects/DED/Onegin /home/tester/CLionProjects/DED/Onegin/cmake-build-debug /home/tester/CLionProjects/DED/Onegin/cmake-build-debug /home/tester/CLionProjects/DED/Onegin/cmake-build-debug/CMakeFiles/Onegin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Onegin.dir/depend

