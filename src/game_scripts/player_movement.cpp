#include "game_scripts/player_movement.h"

PlayerMovement::PlayerMovement(float speed, float acc) {
    this->speed = speed;
    this->acc = acc;
}

void PlayerMovement::start() {
    transform = gameobject->get_component<TransformComponent>();
    rb = gameobject->get_component<Rigidbody2D>();
}

void PlayerMovement::update(float deltaTime) {
    if(transform != nullptr && rb != nullptr) {
        if(IsKeyDown(KEY_RIGHT)) {
            rb->velocity.x = MoveTowards(rb->velocity.x, speed, acc*deltaTime);
        }
        else if(IsKeyDown(KEY_LEFT)) {
            rb->velocity.x = MoveTowards(rb->velocity.x, -speed, acc*deltaTime);
        }
        else {
            rb->velocity.x = MoveTowards(rb->velocity.x, 0, acc*deltaTime);
        }
        
        if(IsKeyDown(KEY_DOWN)) {
            rb->velocity.y = MoveTowards(rb->velocity.y, speed, acc*deltaTime);
        }
        else if(IsKeyDown(KEY_UP)) {
            rb->velocity.y = MoveTowards(rb->velocity.y, -speed, acc*deltaTime);
        }
        else {
            rb->velocity.y = MoveTowards(rb->velocity.y, 0, acc*deltaTime);
        }
    }
}

void PlayerMovement::on_collision_enter_2d(GameObject* other) {
    if(other->name == "other_object")
        std::cout<<"TOUCHING "<<other->name.c_str()<<" ";
}