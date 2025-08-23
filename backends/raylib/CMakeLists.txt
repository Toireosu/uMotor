cmake_minimum_required(VERSION 3.15)

add_library(umotor_backend_raylib
    raylib_engine.cc
)

target_include_directories(umotor_backend_raylib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include       
)

find_package(raylib REQUIRED)
target_link_libraries(umotor_backend_raylib
    PUBLIC raylib
)

target_compile_features(umotor_backend_raylib PUBLIC cxx_std_20)