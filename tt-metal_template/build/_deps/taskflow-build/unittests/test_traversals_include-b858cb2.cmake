if(EXISTS "/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/unittests/test_traversals_tests-b858cb2.cmake")
  include("/home/lpiarulli_tt/stencil_wormhole/tt-metal_compilation_template/build/_deps/taskflow-build/unittests/test_traversals_tests-b858cb2.cmake")
else()
  add_test(test_traversals_NOT_BUILT-b858cb2 test_traversals_NOT_BUILT-b858cb2)
endif()
