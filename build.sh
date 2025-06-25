#!/bin/bash

# For MacOS, no idea if it works for something else
export Qt6_DIR=$(brew --prefix qt6)/lib/cmake/Qt6
export CMAKE_PREFIX_PATH=$(brew --prefix qt6)

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_DESKTOP_GL=ON
make -j$(sysctl -n hw.ncpu)
