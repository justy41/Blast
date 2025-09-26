#include "components/animator.h"

void Animation::update(float deltaTime) {
    duration_left -= deltaTime;
    
    if(duration_left <= 0) {
        duration_left = speed;
        curr++;
        
        if(curr > last) {
            curr = first; // Here it's what causes it to repeat.
        }
    }
}

Rectangle Animation::get_frame(int num_frames_per_row, int cell_width, int cell_height) {
    int x = (curr % num_frames_per_row) * cell_width;
    int y = (curr / num_frames_per_row) * cell_height;
    
    return Rectangle{(float)x, (float)y, (float)cell_width, (float)cell_height};
}

Animator::Animator() {
    
}

Animator* Animator::add_animation(const char* texture_path, int first, int last, float speed) {
    Animation anim = Animation();
    anim.texture = LoadTexture(texture_path);
    anim.first = first;
    anim.last = last;
    anim.curr = first;
    anim.speed = speed;
    anim.duration_left = speed;
    
    animations.push_back(anim);
    return this;
}

void Animator::start() {
    sr = gameobject->get_component<SpriteRenderer>();
}

void Animator::update(float deltaTime) {
    for(auto a : animations) {
        a.update(deltaTime);
    }
}

void Animator::draw(int offset[2]) {
    if(sr != nullptr) {
        // TODO: duplicate SpriteRenderer code or replace the source Rect and texture?
    }
}