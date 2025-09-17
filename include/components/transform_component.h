#pragma once
#include <iostream>
#include <cmath>

#include "raylib.h"
#include "engine.h"

class TransformComponent : public Component {
private:
public:
    Vector2 position;
    float rotation;
    Vector2 scale;
    
    TransformComponent(float x, float y, float rotation, float scaleX, float scaleY);
};