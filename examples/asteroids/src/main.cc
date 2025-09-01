
#include "umotor.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <raylib.h>
#include <raylib_engine.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

// struct EngineImplementation : public umotor::Engine {
//     void open_window() { InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "uMotor Asteroids"); }
//     float get_delta() { return GetFrameTime(); }
//     virtual bool should_stop() { return WindowShouldClose(); }
//     virtual void begin_rending() { BeginDrawing(); ClearBackground(BLACK); }
//     virtual void end_rendering() { EndDrawing(); }
// };

bool GuiButton(std::string text, int x, int y, float font_size) {
    float font_spacing = font_size / 10.0f;
    Vector2 text_size = MeasureTextEx(GetFontDefault(), text.c_str(), font_size, font_spacing);

    Rectangle rect = {
        x - text_size.x / 2 - font_spacing,
        y - text_size.y / 2 - font_spacing,
        text_size.x + font_spacing,
        text_size.y + font_spacing,
    };

    bool mouse_in = CheckCollisionPointRec(GetMousePosition(), rect);

    DrawRectangleRec(
        rect,
        mouse_in ? LIGHTGRAY : GRAY
    );

    DrawTextEx(GetFontDefault(), text.c_str(), (Vector2){ x - text_size.x / 2, y - text_size.y / 2 }, font_size, font_spacing, WHITE);

    return mouse_in && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

class MenuScene : public umotor::Scene {
protected:
    void handle_gui(umotor::Engine& engine, float delta) override;
};

class GameScene : public umotor::Scene {
public:
    int points = 0;
    bool game_over = false;
    GameScene();
protected:
    void handle_gui(umotor::Engine& engine, float delta) override {
        if (game_over) {

            int half_width = WINDOW_WIDTH / 2.0f;


            const char* text = TextFormat("POINTS: %d", points);
            float font_spacing = 80.0f / 10.0f;
            Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 80.0f, font_spacing);
            DrawTextEx(GetFontDefault(), text, (Vector2){half_width - text_size.x / 2, 100}, 80.0f, font_spacing, WHITE);


            if (GuiButton("RESTART", half_width, 450, 60.0f)) {
                engine.switch_scene(std::make_unique<GameScene>());
            }
            if (GuiButton("MENU", half_width, 550, 60.0f)) {
                engine.switch_scene(std::make_unique<MenuScene>());
            }

        } else {
            DrawText(TextFormat("POINTS: %d", points), 0, 0, 60.0f, WHITE);

        }
    }

    static int get_render_index(const umotor::Entity& entity);

    bool compare_render_order(const umotor::Entity& a, const umotor::Entity& b) const override {
        return get_render_index(a) < get_render_index(b);
    }
};

class TransformEntity : public umotor::Entity {
public:
    virtual void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override {
        position = Vector2Add(position, Vector2Scale(velocity, delta));
        rotation += rotational_velocity * delta;

        if (position.x < -50)                  position.x = WINDOW_WIDTH + 50;
        if (position.x > WINDOW_WIDTH + 50)    position.x = -50; 
        if (position.y < -50)                  position.y = WINDOW_HEIGHT + 50;
        if (position.y > WINDOW_HEIGHT + 50)    position.y = -50; 
    }

    const Vector2& get_position() { return position; }
protected:
    Vector2 position = {0, 0};
    Vector2 velocity = {0, 0};
    float rotation = 0.0f;
    float rotational_velocity = 0.0f;
};

class Laser : public TransformEntity {
public:
    Laser(Vector2 position, Vector2 direction) {
        this->position = position;
        velocity = Vector2Scale(Vector2Normalize(direction), speed);
        rotation = atan2f(direction.y, direction.x);
    }

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        
        Matrix t_mat = MatrixTranslate(position.x, position.y, 0.0f);
        Matrix r_mat = MatrixRotateZ(rotation);
        Matrix s_mat = MatrixScale(size, size, 1.0f);

        rlSetMatrixModelview(MatrixMultiply(s_mat, MatrixMultiply(r_mat, t_mat)));

        DrawRectangleRec(
            {
                -0.5, -0.1,
                1.0, 0.2,
            },
            WHITE
        );

        rlDrawRenderBatchActive();

        rlSetMatrixModelview(MatrixIdentity());
    }
private:
    const float speed = 500.0f;
    const float size = 25.0f;
};

class Explosion : public TransformEntity {
public:
    Explosion(Vector2 position) {
        this->position = position;
    }

    void start(umotor::Scene& scene, umotor::Engine& engine) override{
        scene.set_timer([this, &scene]() { scene.remove_entity(this); }, 0.25f, false);
    }

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override{
        size += size_increase * delta;
    }

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override{
        Matrix t_mat = MatrixTranslate(position.x, position.y, 0.0f);
        Matrix r_mat = MatrixRotateZ(rotation);
        Matrix s_mat = MatrixScale(size, size, 1.0f);

        rlSetMatrixModelview(MatrixMultiply(s_mat, MatrixMultiply(r_mat, t_mat)));

        DrawCircle(
            0.0f,
            0.0f,
            0.5,
            RED
        );

        rlDrawRenderBatchActive();

        rlSetMatrixModelview(MatrixIdentity());
    }
private:
    float size = 0.0f;
    const float size_increase = 500.0f;
};

class Asteroid : public TransformEntity {
public:
    Asteroid(Vector2 position) {
        this->has_split = true; 
        size = 25;
        this->position = position;
        rotational_velocity = GetRandomValue(0, 100) / 100.0f;
        velocity = Vector2Scale(Vector2Normalize({ GetRandomValue(-100, 100) / 100.0f, GetRandomValue(-100, 100) / 100.0f}), speed);
    }

    Asteroid() {
        this->has_split = false; 
        size = 50;
        position = {-50, -50};
        rotational_velocity = GetRandomValue(0, 100) / 100.0f;
        velocity = Vector2Scale(Vector2Normalize({ GetRandomValue(-100, 100) / 100.0f, GetRandomValue(-100, 100) / 100.0f}), speed);
    }

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override {
        TransformEntity::update(scene, engine, delta);
        for (auto& laser : scene.get_entities_of_type<Laser>()) {
            if (CheckCollisionPointRec(laser->get_position(), {-size / 2 + position.x, -size / 2 + position.y, size, size})) {
                scene.remove_entity(laser);
                scene.remove_entity(this);
                scene.add_entity(std::make_unique<Explosion>(position));

                dynamic_cast<GameScene*>(&scene)->points += 50;

                if (!has_split) {
                    scene.add_entity(std::make_unique<Asteroid>(position));
                    scene.add_entity(std::make_unique<Asteroid>(position));
                    scene.add_entity(std::make_unique<Asteroid>(position));
                }
                break;
            }
        }
    } 

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        
        Matrix t_mat = MatrixTranslate(position.x, position.y, 0.0f);
        Matrix r_mat = MatrixRotateZ(rotation);
        Matrix s_mat = MatrixScale(size, size, 1.0f);

        rlSetMatrixModelview(MatrixMultiply(s_mat, MatrixMultiply(r_mat, t_mat)));

        DrawRectangleRec(
            {
                -0.5, -0.5,
                1.0, 1.0,
            },
            WHITE
        );

        rlDrawRenderBatchActive();

        rlSetMatrixModelview(MatrixIdentity());
    }

    const float& get_size() { return size; }
private:
    const float speed = 100.0f;
    float size = 50.0f;
    bool has_split = false;
};

class Player : public TransformEntity {
public:
    Player() {
        position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    }

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override {
        TransformEntity::update(scene, engine, delta);
        
        if (IsKeyDown(KEY_A)) rotational_velocity -= rotation_speed * delta;
        if (IsKeyDown(KEY_D)) rotational_velocity += rotation_speed * delta;
        if (IsKeyDown(KEY_W)) {
            velocity.x += cosf(rotation) * delta * acceleration;
            velocity.y += sinf(rotation) * delta * acceleration;
        }
        if (IsKeyDown(KEY_S)) {
            velocity.x -= cosf(rotation) * delta * acceleration;
            velocity.y -= sinf(rotation) * delta * acceleration;
        }

        timer -= delta;
        if (IsKeyDown(KEY_SPACE) && timer < 0.0f) {
            scene.add_entity(std::make_unique<Laser>(position, (Vector2){ cosf(rotation), sinf(rotation) }));
            timer = 0.5f;
        }

        for (auto& asteroid : scene.get_entities_of_type<Asteroid>()) {
            if (CheckCollisionPointRec(position, {-asteroid->get_size() / 2 + asteroid->get_position().x, -asteroid->get_size() / 2 + asteroid->get_position().y, asteroid->get_size(), asteroid->get_size()})) {
                scene.remove_entity(this);
                scene.add_entity(std::make_unique<Explosion>(position));
                dynamic_cast<GameScene*>(&scene)->game_over = true;
                break;
            }
        }
    }

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        
        Matrix t_mat = MatrixTranslate(position.x, position.y, 0.0f);
        Matrix r_mat = MatrixRotateZ(rotation);
        Matrix s_mat = MatrixScale(size, size, 1.0f);

        rlSetMatrixModelview(MatrixMultiply(s_mat, MatrixMultiply(r_mat, t_mat)));

        DrawTriangle(
            {-0.5, -0.3},
            {-0.5, 0.3},
            {0.5, 0.0},
            RED
        );

        rlDrawRenderBatchActive();

        rlSetMatrixModelview(MatrixIdentity());
    }
private:
    const float rotation_speed = 2.0f;
    const float acceleration = 150.0f;
    const float size = 50.0f;
    float timer = 0.0f;
};

GameScene::GameScene() {
    add_entity(std::make_unique<Player>());

    set_timer([this]() { add_entity(std::make_unique<Asteroid>()); }, 3.0f, true);
}

void MenuScene::handle_gui(umotor::Engine& engine, float delta) {
    DrawRectangleGradientV(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, DARKBLUE, DARKPURPLE);

    int half_width = WINDOW_WIDTH / 2.0f;

    const char* text = "uMotor Asteroid";
    float font_spacing = 80.0f / 10.0f;
    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 80.0f, font_spacing);
    DrawTextEx(GetFontDefault(), text, (Vector2){half_width - text_size.x / 2, 200}, 80.0f, font_spacing, WHITE);

    if (GuiButton("START", half_width, 450, 60.0f)) {
        engine.switch_scene(std::make_unique<GameScene>());
    }
    if (GuiButton("EXIT", half_width, 550, 60.0f)) {
        engine.stop();
    }
}

int GameScene::get_render_index(const umotor::Entity& entity) {
    if (dynamic_cast<const Laser*>(&entity)) return 0;
    if (dynamic_cast<const Player*>(&entity)) return 1;
    if (dynamic_cast<const Asteroid*>(&entity)) return 2;
    return 3;
}

int WinMain(void) {

    // EngineImplementation engine;
    Engine engine("uMotor Asteroids", WINDOW_WIDTH, WINDOW_HEIGHT);

    engine.switch_scene(std::make_unique<MenuScene>());

    engine.run();

    return EXIT_SUCCESS;
}