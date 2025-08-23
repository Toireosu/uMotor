project(platformer)


add_executable(platformer 
    src/main.cc
    src/tilemap.cc
    src/player.cc
)

add_custom_command(TARGET platformer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"      # source folder
        "$<TARGET_FILE_DIR:platformer>/assets"  # destination next to exe
)

target_include_directories(platformer
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(platformer PRIVATE umotor umotor_backend_raylib)

target_compile_features(platformer PRIVATE cxx_std_20)