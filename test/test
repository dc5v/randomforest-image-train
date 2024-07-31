#!/bin/bash

SRC=$1
shift

RUN=$(basename "$SRC" .cpp)

g++ -o "$RUN" "$SRC" -I.. $(pkg-config --cflags --libs opencv4)

if [ "$?" -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

./"$RUN" "$@"
