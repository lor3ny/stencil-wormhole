# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /home/lpiarulli_tt/cmake/bin/cmake

# The command to remove a file.
RM = /home/lpiarulli_tt/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build

# Include any dependencies generated for this target.
include _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/flags.make

_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/codegen:
.PHONY : _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/codegen

_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o: _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/flags.make
_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o: /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994/examples/parallel_sort.cpp
_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o: _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o -MF CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o.d -o CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o -c /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994/examples/parallel_sort.cpp

_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parallel_sort.dir/parallel_sort.cpp.i"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994/examples/parallel_sort.cpp > CMakeFiles/parallel_sort.dir/parallel_sort.cpp.i

_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parallel_sort.dir/parallel_sort.cpp.s"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994/examples/parallel_sort.cpp -o CMakeFiles/parallel_sort.dir/parallel_sort.cpp.s

# Object files for target parallel_sort
parallel_sort_OBJECTS = \
"CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o"

# External object files for target parallel_sort
parallel_sort_EXTERNAL_OBJECTS =

_deps/taskflow-build/examples/parallel_sort: _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/parallel_sort.cpp.o
_deps/taskflow-build/examples/parallel_sort: _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/build.make
_deps/taskflow-build/examples/parallel_sort: _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable parallel_sort"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parallel_sort.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/build: _deps/taskflow-build/examples/parallel_sort
.PHONY : _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/build

_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/clean:
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples && $(CMAKE_COMMAND) -P CMakeFiles/parallel_sort.dir/cmake_clean.cmake
.PHONY : _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/clean

_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/depend:
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994/examples /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples /home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/taskflow-build/examples/CMakeFiles/parallel_sort.dir/depend

