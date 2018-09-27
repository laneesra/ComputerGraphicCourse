cmake_minimum_required(VERSION 3.9)
project(lab8)
find_package(OpenGL REQUIRED)

include_directories(glfw/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp shader.h Cube.cpp Cube.h)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${SIFTGPU_INCLUDE_DIR})

add_executable(lab8 ${SOURCE_FILES})

target_link_libraries(lab8  GL ${GLEW_LIBRARIES} glfw)