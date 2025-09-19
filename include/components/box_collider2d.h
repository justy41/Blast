#pragma once
#include <iostream>
#include <cmath>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"

class BoxCollider2D : public Component {
private:
    float width;
    float height;
public:
    TransformComponent* transform;
    Rectangle rect;
    Vector2 rel_position;
    
    BoxCollider2D(float width = 16, float height = 16, float rel_x = 0, float rel_y = 0);
    void start() override;
    void update(float deltaTime) override;
    void draw() override;
};