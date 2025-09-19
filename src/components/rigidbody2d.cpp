#include "components/rigidbody2d.h"

Rigidbody2D::Rigidbody2D(float start_velocity_x, float start_velocity_y, float gravity) {
    velocity = Vector2{start_velocity_x, start_velocity_y};
    this->gravity = gravity;
}

void Rigidbody2D::start() {
    transform = gameobject->get_component<TransformComponent>();
}

void Rigidbody2D::update(float deltaTime) {
    if(transform != nullptr) {
        transform->position.x += velocity.x;
        transform->position.y += velocity.y;
    }
}