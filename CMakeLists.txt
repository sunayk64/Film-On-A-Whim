cmake_minimum_required(VERSION 4.0)
project(Film_On_A_Whim)

set(CMAKE_CXX_STANDARD 17)

add_executable(Film_On_A_Whim
        main.cpp
        DataLoader.cpp
        DataLoader.h
        MaxHeap.cpp
        MaxHeap.h
        Movie.h)