cmake_minimum_required(VERSION 3.16)
project(umotor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

enable_testing()

add_library(umotor STATIC
    src/engine.cc
    src/entity.cc
    src/scene.cc
)

target_include_directories(umotor PUBLIC ${CMAKE_SOURCE_DIR}/include)

option(UMOTOR_BUILD_RAYLIB "Build Raylib backend" ON)

if(UMOTOR_BUILD_RAYLIB)
    add_subdirectory(backends/raylib)
endif()

option(UMOTOR_BUILD_EXAMPLES "Build Examples" OFF)

if (UMOTOR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

option(UMOTOR_BUILD_TESTS "Build Tests" OFF)

if (UMOTOR_BUILD_TESTS)
    add_subdirectory(tests)
endif()
