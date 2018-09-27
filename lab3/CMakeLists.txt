cmake_minimum_required(VERSION 3.9)
project(lab3)
find_package(OpenGL REQUIRED)

include_directories(glfw/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Cube.cpp Cube.h Cylinder.cpp Cylinder.h Point.cpp Point.h)
add_executable(lab3 ${SOURCE_FILES})

target_link_libraries(lab3 glfw GL)