cmake_minimum_required(VERSION 3.9)
project(lab7)
find_package(OpenGL REQUIRED)

include_directories(glfw/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Cylinder.cpp Cylinder.h Point.cpp Point.h Vertex.cpp Vertex.h MyVector.cpp MyVector.h)
find_path(SIFTGPU_INCLUDE_DIR FreeImage.h)
find_library(SIFTGPU_LIBRARY libfreeimageplus)
include_directories(${SIFTGPU_INCLUDE_DIR})

add_executable(lab7 ${SOURCE_FILES})

target_link_libraries(lab7 glfw GL freeimage)