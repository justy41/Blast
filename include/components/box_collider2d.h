#pragma once
#include <iostream>
#include <cmath>
#include <string>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "components/ldtk_world_component.h"
#include "components/rigidbody2d.h"

class BoxCollider2D : public Component {
private:
    float width;
    float height;
public:
    TransformComponent* transform;
    Rigidbody2D* rb;
    LDtkWorldComponent* ldtk_world;
    Rectangle rect;
    Vector2 rel_position;
    std::unordered_map<std::string, bool> collisions;
    
    BoxCollider2D(float width = 16, float height = 16, float rel_x = 0, float rel_y = 0);
    void start() override;
    void update(float deltaTime) override;
    void draw(int offset[2]) override;
};