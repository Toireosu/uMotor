#pragma once

#include "umotor.h"
#include "raylib_engine.h"

class TileMap : public umotor::Entity {
public:
    TileMap(std::string layout, int width, int height);

    void start(umotor::Scene& scene, umotor::Engine& engine) override;

    void render(const umotor::Scene& scene, const umotor::Engine& engine) const override;

    bool is_tile_walkable(Vector2 position);
private:
    Texture* texture;
    std::vector<char> layout;
    int width;
    int height;
};