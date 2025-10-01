#pragma once
#include <iostream>
#include <cmath>
#include <string>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "components/rigidbody2d.h"
#include "components/box_collider2d.h"
#include "components/animator.h"

class PlayerMovement : public Component {
private:
public:
    TransformComponent* transform;
    Rigidbody2D* rb;
    BoxCollider2D* collider;
    Animator* anim;
    Vector2 dir;
    
    float speed;
    float acc;
    float jump_force;
    float initial_gravity;
    
    PlayerMovement(float speed, float acc, float jump_force);
    void start() override;
    void update(float deltaTime) override;
    void on_collision_enter_2d(GameObject* other) override;
};