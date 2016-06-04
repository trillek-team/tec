#!/bin/bash

if [ ! -d "$HOME/lib/$COMPILER" ]; then
    echo "Building dependencies (bullet)"
    mkdir -p $HOME/lib
    cmake ..
    make
    make install DESTDIR="$HOME/lib/$COMPILER"
else
    echo "Dependencies on cache";
fi

