include("/home/lpiarulli_tt/stencil-wormhole/tt-metal_1core_copy/.cpmcache/cpm/CPM_0.40.2.cmake")
CPMAddPackage("NAME;tt-logger;GITHUB_REPOSITORY;tenstorrent/tt-logger;VERSION;1.1.5;OPTIONS;TT_LOGGER_INSTALL ON;TT_LOGGER_BUILD_TESTING OFF")
set(tt-logger_FOUND TRUE)