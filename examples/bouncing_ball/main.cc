#include "raylib_engine.h"
#include <raymath.h>

class BG : public umotor::Entity {
    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        DrawRectangle(0, 0, 480, 480, {0, 200, 180, 255});
    }
};

class Ball : public umotor::Entity {
public:
    void start(umotor::Scene& scene, umotor::Engine& engine) override {
        position = {240, 240};
        velocity = Vector2Scale(Vector2Normalize({ (float)GetRandomValue(-100, 100) / 200, (float)GetRandomValue(-100, 100) / 200 }), speed);
        switch_color();
    }

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override {
        position = Vector2Add(position, Vector2Scale(velocity, delta));

        if (position.x + radius > 480
            || position.x - radius < 0) {
            position.x -= velocity.x * delta;
            velocity.x = -velocity.x;
            switch_color();
        }
        if (position.y + radius > 480
            || position.y - radius < 0) {
            position.y -= velocity.y * delta;
            velocity.y = -velocity.y;
            switch_color();
        }
    }

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        DrawCircle(position.x, position.y, radius, color);
    }

private:
    void switch_color() {
        color = colors[++color_index % 6];
    }

    float radius = 20.0f;
    Vector2 position;
    Vector2 velocity;
    Color color = {255, 255, 255, 255};
    int color_index = -1;
    const Color colors[6] = {
        {255, 80, 0, 255},
        {0, 100, 80, 255},
        {0, 40, 90, 255},
        {150, 40, 0, 255},
        {255, 200, 0, 255},
        {0, 80, 120, 255},
    };
    const float speed = 200.0f;
};

int main(void) {
    Engine engine("uMotor Balls", 480, 480);

    std::unique_ptr<umotor::Scene> scene = std::make_unique<umotor::Scene>();
    
    scene->add_entity<BG>();
    scene->add_entity<Ball>();

    engine.switch_scene(std::move(scene));

    engine.run();
}