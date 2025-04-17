# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/range-v3/e9264fc0c3707c58d8cd5e71e77ca34b61768149")
  file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/range-v3/e9264fc0c3707c58d8cd5e71e77ca34b61768149")
endif()
file(MAKE_DIRECTORY
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-build"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix/tmp"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix/src/range-v3-populate-stamp"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix/src"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix/src/range-v3-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix/src/range-v3-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/range-v3-subbuild/range-v3-populate-prefix/src/range-v3-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
