include("/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/.cpmcache/cpm/CPM_0.40.2.cmake")
CPMAddPackage("NAME;xtensor-blas;GITHUB_REPOSITORY;xtensor-stack/xtensor-blas;GIT_TAG;0.21.0;OPTIONS;CMAKE_MESSAGE_LOG_LEVEL NOTICE;XTENSOR_ENABLE_TESTS OFF")
set(xtensor-blas_FOUND TRUE)