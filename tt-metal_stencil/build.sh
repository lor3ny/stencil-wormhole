#rm -rf build
#cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++-17 -DCMAKE_C_COMPILER=clang-17
make -C build -j64
