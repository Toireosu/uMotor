project(particles)

add_executable(particles 
    main.cc
)

target_link_libraries(particles PRIVATE umotor umotor_backend_raylib)

target_compile_features(particles PRIVATE cxx_std_20)