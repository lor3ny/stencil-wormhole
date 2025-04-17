# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994")
  file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/.cpmcache/taskflow/52063f60902bfeb362fa4616b1394ab5efe30994")
endif()
file(MAKE_DIRECTORY
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-build"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix/tmp"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix/src/taskflow-populate-stamp"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix/src"
  "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix/src/taskflow-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix/src/taskflow-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/build/_deps/taskflow-subbuild/taskflow-populate-prefix/src/taskflow-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
