#pragma once

#include "umotor.h"
#include "raylib_engine.h"

class TileMap;
class Player : public umotor::Entity {
public:
    Player(int x, int y);

    void start(umotor::Scene& scene, umotor::Engine& engine) override;

    void update(umotor::Scene& scene, umotor::Engine& engine, float delta) override;

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override;
private:
    Vector2 position;
    Vector2 velocity;
    Texture* texture;
    TileMap* tile_map;
    bool grounded;
};