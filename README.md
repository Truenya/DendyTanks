# DendyTanks
Currently, training ot this project in game propgramming patterns.

Для сборки в ubuntu

apt update && apt install cmake clang pkg-config libsdl2-dev google-perftools libgtest-dev git 

git clone https://github.com/libsdl-org/SDL_image

cd SDL_image && mkdir build && cd build && cmake .. && make -j8 && make install
