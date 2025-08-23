#include "umotor.h"

#include <cassert>
#include <string>

using namespace umotor;

class FauxEngine : public Engine {
public:
    bool has_run = false;
    bool screen_cleared = false;
    bool buffer_swapped = false;
    bool events_handled = false;

    bool some_setting = false;

    int loops = 0;
protected:
    void open_window() override {
        has_run = true;
    }

    bool should_stop() override {
        return loops >= 5;
    }

    void begin_rending() override {
        screen_cleared = true;
    }

    void end_rendering() override {
        buffer_swapped = true;
        loops++;
    }

    void handle_events() override {
        events_handled = true;
    }

    float get_delta() override {
        return 1.0f;
    }
};

static bool write_line = false;
static bool entities_sorted = false;

class FauxScene : public Scene {
public:
protected:
    void handle_gui(Engine &engine) override {
        write_line = true;
        dynamic_cast<FauxEngine*>(&engine)->some_setting = true;
    }

    bool compare_render_order(const Entity& a, const Entity& b) const override {
        entities_sorted = true;
        return true;
    }
};

static int end_counter = 0;
static int start_counter = 0;
bool entity_rendered = false;

class EndCounterEntity : public Entity {
public:
    void update(Scene& scene, Engine& engine, float delta) override {
        end_counter = scene.get_entities_of_type<Entity>().size();
    }
};

class StartCounterEntity : public Entity {
public:
    void start(Scene& scene, Engine& engine) override {
        start_counter = scene.get_entities_of_type<Entity>().size();
    }
};

class SelfDestructEntity : public Entity {
    void update(Scene& scene, Engine& engine, float delta) override {
        scene.remove_entity(this);
    }
};

class RenderEntity : public Entity {
    void render(const Scene& scene, const Engine& engine) const override {
        entity_rendered = true;
    }
};

int main(void) {

    FauxEngine engine;

    auto scene = std::make_unique<FauxScene>();

    
    
    auto end_counter_entity = std::make_unique<EndCounterEntity>();
    scene->add_entity(std::move(end_counter_entity));
    
    scene->add_entity(std::make_unique<SelfDestructEntity>()); 
    scene->add_entity(std::make_unique<RenderEntity>());
    
    auto start_counter_entity = std::make_unique<StartCounterEntity>();
    scene->add_entity(std::move(start_counter_entity));

    FauxScene* scene_view = scene.get();
    engine.switch_scene(std::move(scene));

    engine.run();

    assert(engine.has_run && "Engine open_window must run!");
    assert(engine.screen_cleared && "Engine begin_rendering must run!");
    assert(engine.buffer_swapped && "Engine end_rendering must run!");
    assert(engine.events_handled && "Engine handle_events must run!");

    assert(write_line && "Scene handle_gui must run!");
    assert(entities_sorted && "Scene compare_render_order must be called!");
    assert(engine.some_setting && "Scene handle_gui should be able to change engine!");

    assert(start_counter == 4 && "Entities at start should be 4!");
    assert(end_counter == 3 && "Entities at the end should be 3!");
    assert(entity_rendered && "Entity render should be called!");


    return EXIT_SUCCESS;
}