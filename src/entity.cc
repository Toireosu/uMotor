#include "umotor.h"

namespace umotor {

bool Entity::is_alive() const {
    return !marked_for_remove;
}

void Entity::start(Scene& scene, Engine& engine) {}
void Entity::update(Scene& scene, Engine& engine, float delta) {}
void Entity::render(const Scene& scene, const Engine& engine) const {}

void Entity::mark_for_remove() {
    marked_for_remove = true;
}

}
