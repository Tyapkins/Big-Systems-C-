cmake_minimum_required(VERSION 3.17)
project(Homework_5)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Homework_5
        RandomNumber.h
        RandomNumber.cpp
        factory.h
        factory.cpp
        main.cpp)