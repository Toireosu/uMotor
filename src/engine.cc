#include "umotor.h"

namespace umotor {

void Engine::run() {
    open_window();

    is_running = true;
    while (is_running && !should_stop()) {
        handle_events();

        begin_rending();

        if (current_scene) current_scene->run(*this, get_delta());

        end_rendering();

        end_of_frame();
    } 
}

void Engine::end_of_frame() {
    if (next_scene)
        current_scene = std::move(next_scene);
}

void Engine::stop() {
    is_running = false;
}

void Engine::switch_scene(std::unique_ptr<Scene> scene) {
    if (is_running)     next_scene = std::move(scene);
    else                current_scene = std::move(scene);
}

void Engine::handle_events() {} 
};


