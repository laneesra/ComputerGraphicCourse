cmake_minimum_required(VERSION 3.9)
project(lab4)
find_package(OpenGL REQUIRED)

include_directories(glfw/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Point.cpp Point.h Rasterisation.cpp Rasterisation.h Edge.cpp Edge.h ActiveEdgeList.h ActiveEdgeList.cpp YGroup.cpp YGroup.h)
add_executable(lab4 ${SOURCE_FILES})

target_link_libraries(lab4 glfw GL)