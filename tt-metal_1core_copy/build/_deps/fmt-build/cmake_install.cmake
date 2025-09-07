# Install script for directory: /home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump-17")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/libfmt.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fmt" TYPE FILE FILES
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/args.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/base.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/chrono.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/color.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/compile.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/core.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/format.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/format-inl.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/os.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/ostream.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/printf.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/ranges.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/std.h"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/fmt/73b5ec45edbd92babfd91c3777a9e1ab9cac8238/include/fmt/xchar.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/fmt-config.cmake"
    "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/fmt-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake"
         "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/CMakeFiles/Export/lib/cmake/fmt/fmt-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/CMakeFiles/Export/lib/cmake/fmt/fmt-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/CMakeFiles/Export/lib/cmake/fmt/fmt-targets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/build/_deps/fmt-build/fmt.pc")
endif()

