#pragma once
#include <iostream>
#include <cmath>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"

class Rigidbody2D : public Component {
private:
public:
    TransformComponent* transform;
    Vector2 velocity;
    float gravity;
    
    Rigidbody2D(float start_velocity_x = 0, float start_velocity_y = 0, float gravity = 1);
    void start() override;
    void update(float deltaTime) override;
};