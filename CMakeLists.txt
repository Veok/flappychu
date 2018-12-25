cmake_minimum_required(VERSION 3.13)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Project1)

add_executable(Project1
        Project1/Main.cpp
        Project1/birdchu.bmp
        Project1/board.bmp
        Project1/endgame.bmp
        Project1/lazy.ttf
        Project1/libfreetype-6.dll
        Project1/libjpeg-9.dll
        Project1/libpng16-16.dll
        Project1/libtiff-5.dll
        Project1/libwebp-7.dll
        Project1/LICENSE.freetype.txt
        Project1/LICENSE.jpeg.txt
        Project1/LICENSE.png.txt
        Project1/LICENSE.tiff.txt
        Project1/LICENSE.webp.txt
        Project1/LICENSE.zlib.txt
        Project1/pipe.bmp
        Project1/pipe2.bmp
        Project1/Project1.vcxproj
        Project1/Project1.vcxproj.filters
        Project1/Project1.vcxproj.user
        Project1/SDL2.dll
        Project1/SDL2_image.dll
        Project1/SDL2_image.lib
        Project1/SDL2_ttf.dll
        Project1/SDL2_ttf.lib
        Project1/zlib1.dll
        Project1.sln
        SDL2.dll
        SDL2.lib
        SDL2main.lib
        SDL2test.lib
        SDL_image.h)
