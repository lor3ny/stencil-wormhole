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
CMAKE_SOURCE_DIR = /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build

# Include any dependencies generated for this target.
include _deps/fmt-build/CMakeFiles/fmt.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/fmt-build/CMakeFiles/fmt.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/fmt-build/CMakeFiles/fmt.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/fmt-build/CMakeFiles/fmt.dir/flags.make

_deps/fmt-build/CMakeFiles/fmt.dir/codegen:
.PHONY : _deps/fmt-build/CMakeFiles/fmt.dir/codegen

_deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.o: _deps/fmt-build/CMakeFiles/fmt.dir/flags.make
_deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.o: /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/format.cc
_deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.o: _deps/fmt-build/CMakeFiles/fmt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.o"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.o -MF CMakeFiles/fmt.dir/src/format.cc.o.d -o CMakeFiles/fmt.dir/src/format.cc.o -c /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/format.cc

_deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/fmt.dir/src/format.cc.i"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/format.cc > CMakeFiles/fmt.dir/src/format.cc.i

_deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/fmt.dir/src/format.cc.s"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/format.cc -o CMakeFiles/fmt.dir/src/format.cc.s

_deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.o: _deps/fmt-build/CMakeFiles/fmt.dir/flags.make
_deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.o: /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/os.cc
_deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.o: _deps/fmt-build/CMakeFiles/fmt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object _deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.o"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.o -MF CMakeFiles/fmt.dir/src/os.cc.o.d -o CMakeFiles/fmt.dir/src/os.cc.o -c /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/os.cc

_deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/fmt.dir/src/os.cc.i"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/os.cc > CMakeFiles/fmt.dir/src/os.cc.i

_deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/fmt.dir/src/os.cc.s"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && /usr/bin/clang++-17 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/src/os.cc -o CMakeFiles/fmt.dir/src/os.cc.s

# Object files for target fmt
fmt_OBJECTS = \
"CMakeFiles/fmt.dir/src/format.cc.o" \
"CMakeFiles/fmt.dir/src/os.cc.o"

# External object files for target fmt
fmt_EXTERNAL_OBJECTS =

_deps/fmt-build/libfmt.a: _deps/fmt-build/CMakeFiles/fmt.dir/src/format.cc.o
_deps/fmt-build/libfmt.a: _deps/fmt-build/CMakeFiles/fmt.dir/src/os.cc.o
_deps/fmt-build/libfmt.a: _deps/fmt-build/CMakeFiles/fmt.dir/build.make
_deps/fmt-build/libfmt.a: _deps/fmt-build/CMakeFiles/fmt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libfmt.a"
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && $(CMAKE_COMMAND) -P CMakeFiles/fmt.dir/cmake_clean_target.cmake
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fmt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/fmt-build/CMakeFiles/fmt.dir/build: _deps/fmt-build/libfmt.a
.PHONY : _deps/fmt-build/CMakeFiles/fmt.dir/build

_deps/fmt-build/CMakeFiles/fmt.dir/clean:
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build && $(CMAKE_COMMAND) -P CMakeFiles/fmt.dir/cmake_clean.cmake
.PHONY : _deps/fmt-build/CMakeFiles/fmt.dir/clean

_deps/fmt-build/CMakeFiles/fmt.dir/depend:
	cd /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238 /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build /home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/fmt-build/CMakeFiles/fmt.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/fmt-build/CMakeFiles/fmt.dir/depend

