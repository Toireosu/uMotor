#include "tilemap.h"
   
TileMap::TileMap(std::string layout, int width, int height) {

    for (int i = 0; i < width * height; i++) {
        this->layout.push_back(layout.at(i));
    }

    this->width = width;
    this->height = height;
}

void TileMap::start(umotor::Scene& scene, umotor::Engine& engine) {
    texture = engine.get_asset<Texture>("assets/spritesheet.png");
}

void TileMap::render(const umotor::Scene& scene, const umotor::Engine& engine) const {
    ClearBackground({ 150, 0, 50, 255});

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char tile_id = this->layout[x + y * width];
            Rectangle source_rect;
            switch (tile_id) {
                case '#':
                    source_rect = {32, 0, 16, 16};
                break;
                case ' ':
                continue;
            };

            DrawTexturePro(
                *texture,
                source_rect,
                {
                    x * 16.0f, y * 16.0f,
                    16, 16
                },
                {0, 0},
                0.0f,
                WHITE
            );
        }
    }
}

bool TileMap::is_tile_walkable(Vector2 position) {
    if (position.x < 0 || position.x >= width || position.y < 0 || position.y >= height) return false;

    return layout[(int)position.x + (int)(position.y) * width] == ' ';
}