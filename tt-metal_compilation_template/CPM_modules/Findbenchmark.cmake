include("/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/cpm/CPM_0.40.2.cmake")
CPMAddPackage("NAME;benchmark;GITHUB_REPOSITORY;google/benchmark;GIT_TAG;v1.9.1;OPTIONS;CMAKE_MESSAGE_LOG_LEVEL NOTICE;BENCHMARK_USE_LIBCXX ;BENCHMARK_ENABLE_TESTING OFF")
set(benchmark_FOUND TRUE)