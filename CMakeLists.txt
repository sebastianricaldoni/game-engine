cmake_minimum_required(VERSION 3.20)

project(first_window LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)

add_executable(first_window)
target_sources(first_window PRIVATE
    src/main.cpp
    src/rendering/Geometry.cpp
    src/rendering/Geometry.h
)
target_link_libraries(first_window PRIVATE SDL3::SDL3)
