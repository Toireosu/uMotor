#pragma once

#include <umotor.h>
#include <raylib.h>
#include <string>

struct Engine : public umotor::Engine {
public:
    Engine(std::string title, int width, int height);
    int get_view_width() { return view_width; }
    int get_view_height() { return view_height; }
protected:
    void open_window() override;
    float get_delta() override;
    bool should_stop() override;
    void begin_rending() override;
    void end_rendering() override;
    void handle_events() override;
private:
    std::string window_title;
    int view_width = 0;
    int view_height = 0;
    RenderTexture view_texture;
    float view_scale = 1.0f;
    Vector2 view_position = {0, 0};
};