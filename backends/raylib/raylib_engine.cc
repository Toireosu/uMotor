#include "raylib_engine.h"
#include <raylib.h>
    
Engine::Engine(std::string title, int width, int height) {
    view_width = width;
    view_height = height;
    window_title = title;
}

static std::unique_ptr<Texture> load_texture(const char* path) {
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    *texture = LoadTexture(path);
    return std::move(texture);
}

void Engine::open_window() { 
    InitWindow(view_width, view_height, window_title.c_str()); 
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    view_texture = LoadRenderTexture(view_width, view_height);

    register_asset_manager<Texture, const char*>(load_texture);
}

float Engine::get_delta() { return GetFrameTime(); }

bool Engine::should_stop() { return WindowShouldClose(); }

void Engine::begin_rending() { 
    BeginDrawing(); 
    ClearBackground(BLACK); 
    BeginTextureMode(view_texture);
    ClearBackground(BLACK);
}

void Engine::end_rendering() { 
    EndTextureMode();

    DrawTexturePro(
        view_texture.texture,
        {
            0,
            0,
            1.0f * view_texture.texture.width,
            -1.0f * view_texture.texture.height,
        },
        {
            view_position.x,
            view_position.y,
            view_scale * view_texture.texture.width,
            view_scale * view_texture.texture.height,
        },
        { 0.0f, 0.0f },
        0.0f, 
        WHITE
    );

    EndDrawing(); 
}

void Engine::handle_events() {
    if (IsWindowResized() || was_resized) {
        view_scale = (float)GetScreenWidth() / view_width;
        if (view_scale * view_height > GetScreenHeight()) {
            view_scale = (float)GetScreenHeight() / view_height;
            view_position = {(GetScreenWidth() - (view_scale * view_width)) / 2, 0};

        } else {
            view_position = {0, (GetScreenHeight() - (view_scale * view_height)) / 2};
        }

        SetMouseOffset(-view_position.x, -view_position.y);
        SetMouseScale(1 / view_scale, 1 / view_scale);
        was_resized = false;
    }
}