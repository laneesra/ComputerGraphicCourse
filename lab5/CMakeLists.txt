cmake_minimum_required(VERSION 3.9)
project(lab5)
find_package(OpenGL REQUIRED)

include_directories(glfw/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Point.cpp Point.h Clipping.cpp Clipping.h Edge.cpp Edge.h)
add_executable(lab5 ${SOURCE_FILES})

target_link_libraries(lab5 glfw GL)