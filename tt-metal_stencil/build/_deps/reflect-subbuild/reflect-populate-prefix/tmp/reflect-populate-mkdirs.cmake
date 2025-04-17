# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/reflect/e75434c4c5f669e4a74e4d84e0a30d7249c1e66f")
  file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/reflect/e75434c4c5f669e4a74e4d84e0a30d7249c1e66f")
endif()
file(MAKE_DIRECTORY
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-build"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix/tmp"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix/src/reflect-populate-stamp"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix/src"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix/src/reflect-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix/src/reflect-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/reflect-subbuild/reflect-populate-prefix/src/reflect-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
