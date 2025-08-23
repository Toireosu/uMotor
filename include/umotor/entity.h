#pragma once

namespace umotor {
class Engine;
class Scene;
class Entity {
public:
    virtual ~Entity() = default;
    bool is_alive() const;
    virtual void start(Scene& scene, Engine& engine);
    virtual void update(Scene& scene, Engine& engine, float delta);
    virtual void render(const Scene& scene, const Engine& engine) const;
private:
    bool marked_for_remove = false;
friend class Scene;
    void mark_for_remove();
};

};