cmake_minimum_required(VERSION 3.28)
project(FileEncrypter)

set(CMAKE_CXX_STANDARD 14)

add_executable(FileEncrypter
        main.cpp
        Util.cpp
        Util.h)
