include("/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/cpm/CPM_0.40.2.cmake")
CPMAddPackage("NAME;flatbuffers;GITHUB_REPOSITORY;google/flatbuffers;GIT_TAG;v24.3.25;OPTIONS;FLATBUFFERS_BUILD_FLATC ON;FLATBUFFERS_BUILD_TESTS OFF;FLATBUFFERS_SKIP_MONSTER_EXTRA ON;FLATBUFFERS_STRICT_MODE ON")
set(flatbuffers_FOUND TRUE)