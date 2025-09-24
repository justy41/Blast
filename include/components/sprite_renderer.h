#pragma once
#include <iostream>
#include <cmath>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"

class SpriteRenderer : public Component {
private:
public:
    Texture2D texture;
    TransformComponent* transform;
    Vector2 origin;
    Color tint;
    float depth;
    
    SpriteRenderer(const char* texture_path, float depth = 1, float originX = 0, float originY = 0, Color tint = WHITE);
    void start() override;
    void draw(int offset[2]) override;
};