#include "components/rigidbody2d.h"
#include "components/box_collider2d.h"

Rigidbody2D::Rigidbody2D(float start_velocity_x, float start_velocity_y, float gravity) {
    velocity = Vector2{start_velocity_x, start_velocity_y};
    this->gravity = gravity;
}

void Rigidbody2D::start() {
    transform = gameobject->get_component<TransformComponent>();
    collider = gameobject->get_component<BoxCollider2D>();
}

void Rigidbody2D::update(float deltaTime) {
    // !!!
    // If the gameobject does not have a BoxCollider2D component then handle the position and velocity here.
    // If the gameobject has one, then the BoxCollider2D component will handle the physics.
    if(collider == nullptr) {
        if(transform != nullptr) {
            transform->position.x += velocity.x * deltaTime;
            
            velocity.y += gravity * deltaTime;
            transform->position.y += velocity.y * deltaTime;
        }
    }
}