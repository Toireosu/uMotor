project(bouncing_ball)

add_executable(bouncing_ball 
    main.cc
)

target_link_libraries(bouncing_ball PRIVATE umotor umotor_backend_raylib)

target_compile_features(bouncing_ball PRIVATE cxx_std_20)