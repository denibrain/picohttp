# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
CMAKE_SOURCE_DIR = /home/denis/Solutions/Cpp/http

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/denis/Solutions/Cpp/http

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/denis/Solutions/Cpp/http/CMakeFiles /home/denis/Solutions/Cpp/http/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/denis/Solutions/Cpp/http/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named final

# Build rule for target.
final: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 final
.PHONY : final

# fast build rule for target.
final/fast:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/build
.PHONY : final/fast

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/main.cpp.s
.PHONY : main.cpp.s

src/daemon.o: src/daemon.cpp.o
.PHONY : src/daemon.o

# target to build an object file
src/daemon.cpp.o:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/daemon.cpp.o
.PHONY : src/daemon.cpp.o

src/daemon.i: src/daemon.cpp.i
.PHONY : src/daemon.i

# target to preprocess a source file
src/daemon.cpp.i:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/daemon.cpp.i
.PHONY : src/daemon.cpp.i

src/daemon.s: src/daemon.cpp.s
.PHONY : src/daemon.s

# target to generate assembly for a file
src/daemon.cpp.s:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/daemon.cpp.s
.PHONY : src/daemon.cpp.s

src/filetype.o: src/filetype.cpp.o
.PHONY : src/filetype.o

# target to build an object file
src/filetype.cpp.o:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/filetype.cpp.o
.PHONY : src/filetype.cpp.o

src/filetype.i: src/filetype.cpp.i
.PHONY : src/filetype.i

# target to preprocess a source file
src/filetype.cpp.i:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/filetype.cpp.i
.PHONY : src/filetype.cpp.i

src/filetype.s: src/filetype.cpp.s
.PHONY : src/filetype.s

# target to generate assembly for a file
src/filetype.cpp.s:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/filetype.cpp.s
.PHONY : src/filetype.cpp.s

src/request.o: src/request.cpp.o
.PHONY : src/request.o

# target to build an object file
src/request.cpp.o:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/request.cpp.o
.PHONY : src/request.cpp.o

src/request.i: src/request.cpp.i
.PHONY : src/request.i

# target to preprocess a source file
src/request.cpp.i:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/request.cpp.i
.PHONY : src/request.cpp.i

src/request.s: src/request.cpp.s
.PHONY : src/request.s

# target to generate assembly for a file
src/request.cpp.s:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/request.cpp.s
.PHONY : src/request.cpp.s

src/server.o: src/server.cpp.o
.PHONY : src/server.o

# target to build an object file
src/server.cpp.o:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/server.cpp.o
.PHONY : src/server.cpp.o

src/server.i: src/server.cpp.i
.PHONY : src/server.i

# target to preprocess a source file
src/server.cpp.i:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/server.cpp.i
.PHONY : src/server.cpp.i

src/server.s: src/server.cpp.s
.PHONY : src/server.s

# target to generate assembly for a file
src/server.cpp.s:
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/src/server.cpp.s
.PHONY : src/server.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... final"
	@echo "... rebuild_cache"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... src/daemon.o"
	@echo "... src/daemon.i"
	@echo "... src/daemon.s"
	@echo "... src/filetype.o"
	@echo "... src/filetype.i"
	@echo "... src/filetype.s"
	@echo "... src/request.o"
	@echo "... src/request.i"
	@echo "... src/request.s"
	@echo "... src/server.o"
	@echo "... src/server.i"
	@echo "... src/server.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
