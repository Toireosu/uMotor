#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "umotor/entity.h"

namespace umotor {

class Engine;
class Scene {
public:
    virtual ~Scene() = default;
    template <typename T, typename... Args>
        requires std::derived_from<T, Entity>
    T& add_entity(Args&&... args) {
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *entity; 
        add_buffer.push_back(std::move(entity));
        return ref;
    }
    void add_entity(std::unique_ptr<Entity> entity);
    void remove_entity(Entity* entity);
    void set_timer(std::function<void()> callback, float time, bool repeat);

    template <typename EntityType>
    std::vector<EntityType*> get_entities_of_type() const {
        std::vector<EntityType*> matches;
        for (auto& entity : entities) {
            EntityType* t = dynamic_cast<EntityType*>(entity.get());
            if (t && entity->is_alive()) matches.push_back(t);
        }

        for (auto& entity : add_buffer) {
            EntityType* t = dynamic_cast<EntityType*>(entity.get());
            if (t && entity->is_alive()) matches.push_back(t);
        }
    
        return matches;
    }

    template <typename EntityType>
    EntityType* get_first_entity_of_type() const {
        std::vector<EntityType*> entities = get_entities_of_type<EntityType>();
    
        return entities.empty() ? nullptr : entities[0];
    }

protected:
    virtual void handle_gui(Engine &engine);
    virtual bool compare_render_order(const Entity& a, const Entity& b) const; 
private:
    struct Timer {
        std::function<void()> callback;
        float time;
        float start_time;
        bool repeat;
    };

    void add_entity_immediate(std::unique_ptr<Entity> entity, Engine& engine);
    void remove_entity_immediate(Entity* entity);
    void update_entities(Engine& engine, float delta);
    void render_entities(const Engine& engine);
    void start_of_frame(Engine& engine);
    void end_of_frame();

    void handle_timers(float delta);

    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> add_buffer;
    std::vector<Timer> timers;
    std::vector<Entity*> remove_buffer;
friend class Engine;
    void run(Engine &engine, float delta);
};

};