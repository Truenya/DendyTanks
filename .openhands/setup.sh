#!/bin/env bash
sudo apt update
sudo apt install cmake clang pkg-config libsdl2-dev libsdl2-image-dev google-perftools libgtest-dev git 
mkdir -p build
cd build && cmake .. && make -j8 && make install
