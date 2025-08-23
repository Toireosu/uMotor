#include "player.h"
#include <raymath.h>
#include "tilemap.h"

Player::Player(int x, int y) {
    position = { 1.0f * x, 1.0f * y };
    velocity = {0, 0};
}

void Player::start(umotor::Scene& scene, umotor::Engine& engine) {
    texture = engine.get_asset<Texture>("assets/spritesheet.png");
    tile_map = scene.get_entities_of_type<TileMap>()[0];
}

static bool check_collision(Vector2 position, TileMap* tile_map) {
    const float thickness = 0.47f;
    const float height = 1.8f;
    const float half_height = height / 2;

    return
        !tile_map->is_tile_walkable({ position.x, position.y }) || 
        !tile_map->is_tile_walkable({ position.x - thickness, position.y }) ||
        !tile_map->is_tile_walkable({ position.x + thickness, position.y }) ||
        !tile_map->is_tile_walkable({ position.x - thickness, position.y - half_height }) ||
        !tile_map->is_tile_walkable({ position.x + thickness, position.y - half_height }) ||
        !tile_map->is_tile_walkable({ position.x - thickness, position.y - height }) ||
        !tile_map->is_tile_walkable({ position.x + thickness, position.y - height });
}

void Player::update(umotor::Scene& scene, umotor::Engine& engine, float delta) {
    const float speed = 50.0f;
    const float friction = 15.0f;
    const float gravity = 5.0f;
    const float jump_height = 5.0f;

    
    if (IsKeyDown(KEY_A)) velocity.x -= speed * delta;
    if (IsKeyDown(KEY_D)) velocity.x += speed * delta;
    if (IsKeyDown(KEY_SPACE) && grounded) velocity.y -= jump_height;
    
    velocity.x -= velocity.x * friction * delta;
    velocity.y += gravity * delta;
    
    Vector2 next_pos = { position.x + velocity.x * delta, position.y };
    if (check_collision(next_pos, tile_map)) {
        next_pos.x = position.x;
        velocity.x = 0;
    }
    
    grounded = false;
    next_pos.y += velocity.y * delta;
    if (check_collision(next_pos, tile_map)) {
        next_pos.y = position.y;
        velocity.y = 0;
        grounded = true;
    }
    
    position = next_pos;
}

void Player::render(const umotor::Scene& scene, const umotor::Engine& engine) const {
    DrawTexturePro(
        *texture,
        {0, 0, 32, 32},
        {position.x * 16.0f, position.y * 16.0f, 32, 32},
        {16, 32},
        0.0f,
        WHITE
    );
}