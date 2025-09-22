#pragma once
#include <iostream>
#include <cmath>
#include <string>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "components/rigidbody2d.h"

class PlayerMovement : public Component {
private:
public:
    TransformComponent* transform;
    Rigidbody2D* rb;
    
    float speed;
    float acc;
    
    PlayerMovement(float speed, float acc);
    void start() override;
    void update(float deltaTime) override;
    void on_collision_enter_2d(GameObject* other) override;
};