#!/bin/bash
rm CMakeCache.txt && cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ && make -j8
