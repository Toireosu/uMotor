project(asteroids)

add_executable(asteroids 
    src/main.cc
)

target_link_libraries(asteroids PRIVATE umotor umotor_backend_raylib)

target_compile_features(asteroids PRIVATE cxx_std_20)