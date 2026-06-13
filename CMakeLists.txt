cmake_minimum_required(VERSION 3.20)

project(razor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC vendor/glad/src/glad.c)
add_executable(razor src/main.cpp src/platform.cpp)

target_compile_options(razor PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)

target_include_directories(glad PUBLIC vendor/glad/include)
target_include_directories(razor PRIVATE include)

target_link_libraries(razor PRIVATE glad glfw OpenGL::GL)


