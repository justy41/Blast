#include "components/sprite_renderer.h"

SpriteRenderer::SpriteRenderer(const char* texture_path, float originX, float originY, Color tint) {
    texture = LoadTexture(texture_path);
    origin = Vector2{originX, originY};
    this->tint = tint;
}

void SpriteRenderer::start() {
    transform = gameobject->get_component<TransformComponent>();
}

void SpriteRenderer::draw(int offset[2]) {
    DrawTexturePro(
        texture,
        Rectangle{0, 0, (float)texture.width, (float)texture.height},
        Rectangle{transform->position.x - offset[0], transform->position.y - offset[1], texture.width*transform->scale.x, texture.height*transform->scale.y},
        origin,
        transform->rotation,
        tint
    );
}