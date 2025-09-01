#include <raylib_engine.h>
#include <raymath.h>

class ParticleScene : public umotor::Scene {
protected:
    void handle_gui(umotor::Engine& engine, float delta) override {
        DrawFPS(0, 0);
    }
};

class Particle : public umotor::Entity {
public:
    Particle(Vector2 position, Vector2 velocity) {
        color = { (unsigned char)GetRandomValue(100, 255), (unsigned char)GetRandomValue(100, 255), (unsigned char)GetRandomValue(100, 255), 255};
        this->position = position;
        this->velocity = velocity;
    }

    void start(umotor::Scene& scene, umotor::Engine& engine) override {
        scene.set_timer([this, &scene](){
            scene.remove_entity(this);
        },
        10.0f,
        false);
    }

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override {
        velocity = Vector2Add(velocity, {0, gravity * delta});
        position = Vector2Add(position, Vector2Scale(velocity, delta));
    }
    
    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        DrawRectangle(
            position.x,
            position.y,
            1,
            1,
            color
        );
    }
private:
    Color color;
    Vector2 position;
    Vector2 velocity; 
    const float gravity = 30;
};

class ParticleSpawner : public umotor::Entity {
    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 position = GetMousePosition();
            for (int i = 0; i < 50; i++) {
                Vector2 velocity = Vector2Scale(Vector2Normalize({ (float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50) }), (float)GetRandomValue(30, 100));
                scene.add_entity<Particle>(position, velocity);
            }
        }
    }

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        ClearBackground({50, 50, 50, 255});
    }
};

int main(void) {
    Engine engine("uMotor Particles", 360, 240);

    ParticleScene& scene = engine.switch_scene<ParticleScene>();
    scene.add_entity<ParticleSpawner>();

    engine.run();
}