#! /bin/bash
mkdir -p build
cd build
cmake .. -DWITH_TESTS=yes
make -j
make test
