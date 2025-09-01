#include <raylib_engine.h>
#include <cmath>
#include <raymath.h>
#include <rlgl.h>

class Entity : public umotor::Entity {
public:
    void start(umotor::Scene& scene, umotor::Engine& engine) override {
        SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
        dynamic_cast<Engine&>(engine).force_handle_resize();
    }

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override {
        time += delta;
    }

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override {
        unsigned char strobe = fabs(sin_triangle(time * 0.7f)) * 100;
        unsigned char r = strobe + 50;
        unsigned char g = strobe / 2 + 50;
        unsigned char b = strobe + 100;
        ClearBackground({ r, g, b, 255 });

        draw_rotated_text({255, 255, 255, 50}, sinf(time - 0.4f) * 0.3f);
        draw_rotated_text({255, 255, 255, 100}, sinf(time - 0.3f) * 0.3f);
        draw_rotated_text({255, 255, 255, 150}, sinf(time - 0.2f) * 0.3f);
        draw_rotated_text({255, 255, 255, 200}, sinf(time - 0.1f) * 0.3f);
        draw_rotated_text({50, 50, 50, 255}, sinf(time - 0.05f) * 0.3f);
        draw_rotated_text(WHITE, sinf(time) * 0.3f);

    }
    private:
    void draw_rotated_text(Color c, float rotation) const {
        
        const float font_size = 40.0f;
        const float font_spacing = font_size / 10.0f;
        
        Vector2 text_size = MeasureTextEx(GetFontDefault(), "Hello, uMotor!", font_size, font_spacing);
        Matrix firstTranslate = MatrixTranslate(-text_size.x / 2, -text_size.y / 2, 0.0f);
        Matrix rotationMatrix = MatrixRotateZ(rotation);
        Matrix lastTranslate = MatrixTranslate(360 / 2, 360 / 2, 0);
        
        rlSetMatrixModelview(MatrixMultiply(MatrixMultiply(firstTranslate, rotationMatrix), lastTranslate));
        DrawTextEx(GetFontDefault(), "Hello, uMotor!", { 0, 0}, font_size, font_spacing, c);

        rlDrawRenderBatchActive();

        rlSetMatrixModelview(MatrixIdentity());
    }

    float sin_triangle(float x) const {
        return (2 / 3.14f) * asinf(sinf(3.14 * x));
    }

    float time = 0.0f;
};

int main() {
    Engine engine("Hello, uMotor!", 360, 360);

    auto scene = std::make_unique<umotor::Scene>();
    scene->add_entity(std::make_unique<Entity>());
    engine.switch_scene(std::move(scene));

    engine.run();
}