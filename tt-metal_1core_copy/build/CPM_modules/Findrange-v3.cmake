include("/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/cpm/CPM_0.40.2.cmake")
CPMAddPackage("NAME;range-v3;GITHUB_REPOSITORY;ericniebler/range-v3;GIT_TAG;0.12.0;PATCHES;range-v3.patch;OPTIONS;CMAKE_BUILD_TYPE Release;CMAKE_MESSAGE_LOG_LEVEL NOTICE")
set(range-v3_FOUND TRUE)