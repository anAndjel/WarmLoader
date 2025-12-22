#!/usr/bin/env bash

cd /home/alex/coding/WarmLoader || exit 1

set -e

mkdir -p build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -B build -S .
cmake --build build --target warmloader -- -j "$(nproc)"
