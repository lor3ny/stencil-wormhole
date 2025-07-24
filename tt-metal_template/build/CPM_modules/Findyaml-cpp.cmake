include("/home/lpiarulli_tt/stencil_wormhole/tt-metal_template/.cpmcache/cpm/CPM_0.40.2.cmake")
CPMAddPackage("NAME;yaml-cpp;GITHUB_REPOSITORY;jbeder/yaml-cpp;GIT_TAG;0.8.0;PATCHES;yaml-cpp.patch;OPTIONS;YAML_CPP_BUILD_TESTS OFF;YAML_CPP_BUILD_TOOLS OFF;YAML_BUILD_SHARED_LIBS OFF")
set(yaml-cpp_FOUND TRUE)