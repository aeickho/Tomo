#!/bin/bash
rm CMakeCache.txt 2&> /dev/null ; cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ && make clean && make -j
