#!/bin/sh
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../04-Source/m4/SDK_2.8.0_EVK-MIMX8MM/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=debug  .
make -j 2>&1 | tee build_log.txt
