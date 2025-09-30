#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"

class SpriteRenderer;

class Animation {
private:
public:
    int first, last, curr, num_frames_per_row, cell_width, cell_height;
    float speed, duration_left;
    Texture2D texture;
    std::string name;
    
    Animation() = default;
    void update(float deltaTime);
    Rectangle get_frame(int num_frames_per_row, int cell_width, int cell_height);
};

class Animator : public Component {
private:
public:
    SpriteRenderer* sr;
    std::unordered_map<std::string, Animation> animations;
    Animation* current_anim;
    bool paused;
    int count;
    
    Animator();
    Animator* add_animation(const char* texture_path, const std::string& name, int num_frames_per_row, int cell_width, int cell_height, float speed);
    void play(const std::string& name);
    void pause();
    void start() override;
    void update(float deltaTime) override;
    void draw(int offset[2]) override;
};