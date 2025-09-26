#include "components/sprite_renderer.h"

SpriteRenderer::SpriteRenderer(const char* texture_path, float depth, float originX, float originY, Color tint) {
    texture = LoadTexture(texture_path);
    this->depth = depth;
    origin = Vector2{originX, originY};
    this->tint = tint;
    src = Rectangle{0, 0, (float)texture.width, (float)texture.height};
}

void SpriteRenderer::start() {
    transform = gameobject->get_component<TransformComponent>();
}

void SpriteRenderer::draw(int offset[2]) {
    DrawTexturePro(
        texture,
        src,
        Rectangle{transform->position.x - offset[0]*depth, transform->position.y - offset[1]*depth, texture.width*transform->scale.x, texture.height*transform->scale.y},
        origin,
        transform->rotation,
        tint
    );
}