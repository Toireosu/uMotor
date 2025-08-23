project(hello_umotor)

add_executable(hello_umotor 
    main.cc
)

target_link_libraries(hello_umotor PRIVATE umotor umotor_backend_raylib)

target_compile_features(hello_umotor PRIVATE cxx_std_20)