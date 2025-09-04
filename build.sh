#!/bin/bash
set -e

mkdir -p build
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
ln -sf build/compile_commands.json .
cd build
./engine
