#pragma once
#include <iostream>
#include <cmath>
#include <vector>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "components/sprite_renderer.h"

class Animation {
private:
public:
    int first, last, curr;
    float speed, duration_left;
    Texture2D texture;
    
    Animation() = default;
    void update(float deltaTime);
    Rectangle get_frame(int num_frames_per_row, int cell_width, int cell_height);
};

class Animator : public Component {
private:
public:
    Texture2D texture;
    TransformComponent* transform;
    SpriteRenderer* sr;
    std::vector<Animation> animations;
    
    Animator();
    Animator* add_animation(const char* texture_path, int first, int last, float speed);
    void start() override;
    void update(float deltaTime) override;
    void draw(int offset[2]) override;
};