#!/bin/bash

set -e

if [ ! -d "$HOME/cmake/bin" ]; then
    #Download CMake 3.2
    wget --no-check-certificate http://www.cmake.org/files/v3.2/cmake-3.2.3-Linux-x86_64.tar.gz
    mkdir -p $HOME/cmake
    tar zxf cmake-3.2.3-Linux-x86_64.tar.gz -C $HOME/cmake --strip-components=1
else
    echo "CMake - Using cached directory";
fi

