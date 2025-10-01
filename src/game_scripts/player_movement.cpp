#include "game_scripts/player_movement.h"

PlayerMovement::PlayerMovement(float speed, float acc, float jump_force) {
    this->speed = speed;
    this->acc = acc;
    this->jump_force = jump_force;
}

void PlayerMovement::start() {
    transform = gameobject->get_component<TransformComponent>();
    rb = gameobject->get_component<Rigidbody2D>();
    collider = gameobject->get_component<BoxCollider2D>();
    anim = gameobject->get_component<Animator>();
    dir = Vector2{0, 0};
    
    initial_gravity = rb->gravity;
}

void PlayerMovement::update(float deltaTime) {
    if(transform != nullptr && rb != nullptr) {
        if(IsKeyDown(KEY_RIGHT)) {
            rb->velocity.x = MoveTowards(rb->velocity.x, speed, acc*deltaTime);
            dir.x = 1;
        }
        else if(IsKeyDown(KEY_LEFT)) {
            rb->velocity.x = MoveTowards(rb->velocity.x, -speed, acc*deltaTime);
            dir.x = -1;
        }
        else {
            rb->velocity.x = MoveTowards(rb->velocity.x, 0, acc*deltaTime);
            dir.x = 0;
        }
        
        if(collider->collisions["down"]) {
            rb->gravity = 1; // Dont make this 0 or the "down" collision check will jumble up.
                             // It's usually good to keep it 1 as it still pushes the gameobject a bit into the ground.
            
            if(IsKeyPressed(KEY_Z)) {
                rb->velocity.y = -jump_force;
            }
        }
        else {
            if(rb->velocity.y < 0) { // If not on ground and moving up (positive y goes down).
                rb->gravity = initial_gravity;
                
                if(IsKeyReleased(KEY_Z)) {
                    rb->velocity.y /= 2; // Cut the jump in 2 if releasing the button mid ascend.
                }
            }
            else {
                rb->gravity = initial_gravity * 2; // When falling apply a multiplier to make the fall faster. Feels better.
            }
        }
        
        // ANIMATIONS
        if(collider->collisions["down"]) {
            if(dir.x != 0) {
                anim->play("run");
            }
            else {
                anim->play("idle");
            }
        }
        else {
            anim->play("idle");
        }
    }
}

void PlayerMovement::on_collision_enter_2d(GameObject* other) {
    // Checking if the other object we collided with is named "other_object".
    if(other->name == "other_object")
        std::cout<<"TOUCHING "<<other->name.c_str()<<" ";
}