# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/james/Code/NetworkHealthMonitor/CentralServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/james/Code/NetworkHealthMonitor/CentralServer

# Include any dependencies generated for this target.
include src/CMakeFiles/central_server.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/central_server.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/central_server.dir/flags.make

src/CMakeFiles/central_server.dir/clientHandler.c.o: src/CMakeFiles/central_server.dir/flags.make
src/CMakeFiles/central_server.dir/clientHandler.c.o: src/clientHandler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/james/Code/NetworkHealthMonitor/CentralServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/central_server.dir/clientHandler.c.o"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/central_server.dir/clientHandler.c.o   -c /home/james/Code/NetworkHealthMonitor/CentralServer/src/clientHandler.c

src/CMakeFiles/central_server.dir/clientHandler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/central_server.dir/clientHandler.c.i"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/james/Code/NetworkHealthMonitor/CentralServer/src/clientHandler.c > CMakeFiles/central_server.dir/clientHandler.c.i

src/CMakeFiles/central_server.dir/clientHandler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/central_server.dir/clientHandler.c.s"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/james/Code/NetworkHealthMonitor/CentralServer/src/clientHandler.c -o CMakeFiles/central_server.dir/clientHandler.c.s

src/CMakeFiles/central_server.dir/clientHandler.c.o.requires:

.PHONY : src/CMakeFiles/central_server.dir/clientHandler.c.o.requires

src/CMakeFiles/central_server.dir/clientHandler.c.o.provides: src/CMakeFiles/central_server.dir/clientHandler.c.o.requires
	$(MAKE) -f src/CMakeFiles/central_server.dir/build.make src/CMakeFiles/central_server.dir/clientHandler.c.o.provides.build
.PHONY : src/CMakeFiles/central_server.dir/clientHandler.c.o.provides

src/CMakeFiles/central_server.dir/clientHandler.c.o.provides.build: src/CMakeFiles/central_server.dir/clientHandler.c.o


src/CMakeFiles/central_server.dir/main.c.o: src/CMakeFiles/central_server.dir/flags.make
src/CMakeFiles/central_server.dir/main.c.o: src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/james/Code/NetworkHealthMonitor/CentralServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/central_server.dir/main.c.o"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/central_server.dir/main.c.o   -c /home/james/Code/NetworkHealthMonitor/CentralServer/src/main.c

src/CMakeFiles/central_server.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/central_server.dir/main.c.i"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/james/Code/NetworkHealthMonitor/CentralServer/src/main.c > CMakeFiles/central_server.dir/main.c.i

src/CMakeFiles/central_server.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/central_server.dir/main.c.s"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/james/Code/NetworkHealthMonitor/CentralServer/src/main.c -o CMakeFiles/central_server.dir/main.c.s

src/CMakeFiles/central_server.dir/main.c.o.requires:

.PHONY : src/CMakeFiles/central_server.dir/main.c.o.requires

src/CMakeFiles/central_server.dir/main.c.o.provides: src/CMakeFiles/central_server.dir/main.c.o.requires
	$(MAKE) -f src/CMakeFiles/central_server.dir/build.make src/CMakeFiles/central_server.dir/main.c.o.provides.build
.PHONY : src/CMakeFiles/central_server.dir/main.c.o.provides

src/CMakeFiles/central_server.dir/main.c.o.provides.build: src/CMakeFiles/central_server.dir/main.c.o


# Object files for target central_server
central_server_OBJECTS = \
"CMakeFiles/central_server.dir/clientHandler.c.o" \
"CMakeFiles/central_server.dir/main.c.o"

# External object files for target central_server
central_server_EXTERNAL_OBJECTS =

bin/central_server: src/CMakeFiles/central_server.dir/clientHandler.c.o
bin/central_server: src/CMakeFiles/central_server.dir/main.c.o
bin/central_server: src/CMakeFiles/central_server.dir/build.make
bin/central_server: src/CMakeFiles/central_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/james/Code/NetworkHealthMonitor/CentralServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../bin/central_server"
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/central_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/central_server.dir/build: bin/central_server

.PHONY : src/CMakeFiles/central_server.dir/build

src/CMakeFiles/central_server.dir/requires: src/CMakeFiles/central_server.dir/clientHandler.c.o.requires
src/CMakeFiles/central_server.dir/requires: src/CMakeFiles/central_server.dir/main.c.o.requires

.PHONY : src/CMakeFiles/central_server.dir/requires

src/CMakeFiles/central_server.dir/clean:
	cd /home/james/Code/NetworkHealthMonitor/CentralServer/src && $(CMAKE_COMMAND) -P CMakeFiles/central_server.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/central_server.dir/clean

src/CMakeFiles/central_server.dir/depend:
	cd /home/james/Code/NetworkHealthMonitor/CentralServer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/james/Code/NetworkHealthMonitor/CentralServer /home/james/Code/NetworkHealthMonitor/CentralServer/src /home/james/Code/NetworkHealthMonitor/CentralServer /home/james/Code/NetworkHealthMonitor/CentralServer/src /home/james/Code/NetworkHealthMonitor/CentralServer/src/CMakeFiles/central_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/central_server.dir/depend

