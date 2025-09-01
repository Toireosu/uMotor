#include "umotor.h"
#include <algorithm>

#include <cassert>

namespace umotor {

void Scene::add_entity(std::unique_ptr<Entity> entity) {
    add_buffer.push_back(std::move(entity));
}

void Scene::remove_entity(Entity* entity) {
    entity->mark_for_remove();
    remove_buffer.push_back(entity);
}

void Scene::set_timer(std::function<void()> callback, float time, bool repeat) {
    timers.push_back((Timer){ .callback = callback, .time = time, .start_time = time, .repeat = repeat});
}

void Scene::handle_gui(Engine &engine, float delta) { }
bool Scene::compare_render_order(const Entity& a, const Entity& b) const { return false; } 

void Scene::add_entity_immediate(std::unique_ptr<Entity> entity, Engine& engine) {
    Entity* entity_view = entity.get();
    entities.push_back(std::move(entity));
    entity_view->start(*this, engine);
}

void Scene::remove_entity_immediate(Entity* entity) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [entity](const std::unique_ptr<Entity>& e) {
                return e.get() == entity;
            }),
        entities.end()
    );
}

void Scene::update_entities(Engine& engine, float delta) {
    for (auto& entity : entities) {
        if (entity->is_alive()) entity->update(*this, engine, delta);
    }
}

void Scene::render_entities(const Engine& engine) {
   std::sort(entities.begin(), entities.end(), 
        [this](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b) {
            return compare_render_order(*a.get(), *b.get());
        });

   for (const auto& entity : entities) {
        if (entity->is_alive()) entity->render(*this, engine);
   }
}

void Scene::start_of_frame(Engine& engine) {
    for (auto& entity : add_buffer) {
        add_entity_immediate(std::move(entity), engine);
    }
    
    add_buffer.clear();
}

void Scene::end_of_frame() {

    for (auto& entity : remove_buffer) {
        remove_entity_immediate(entity);
    }

    remove_buffer.clear();
}

void Scene::run(Engine &engine, float delta) {
    start_of_frame(engine);

    handle_timers(delta);

    update_entities(engine, delta);

    render_entities(engine);

    handle_gui(engine, delta);

    end_of_frame();
}

void Scene::handle_timers(float delta) {
    for (auto it = timers.begin(); it != timers.end();) {
        it->time -= delta;
        if (it->time < 0.0f) {
            it->callback();
            if (it->repeat) {
                it->time = it->start_time;
            } else {
                timers.erase(it);
            }
        } else {
            it++;
        }
    }
}

};