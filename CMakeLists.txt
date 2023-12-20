cmake_minimum_required(VERSION 3.26)
project(catalog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

add_executable(catalog main.cpp)
