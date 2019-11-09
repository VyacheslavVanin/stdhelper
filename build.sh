#! /bin/bash
mkdir -p build
cd build
( cmake .. -DWITH_TESTS=yes -DCMAKE_EXPORT_COMPILE_COMMANDS=yes && cp compile_commands.json .. ) && \
make -j && \
make test
