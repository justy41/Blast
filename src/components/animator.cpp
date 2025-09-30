#include "components/animator.h"
#include "components/sprite_renderer.h"

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

Rectangle Animation::get_frame(int _num_frames_per_row, int _cell_width, int _cell_height) {
    int x = (curr % _num_frames_per_row) * _cell_width;
    int y = (curr / _num_frames_per_row) * _cell_height;
    
    return Rectangle{(float)x, (float)y, (float)_cell_width, (float)_cell_height};
}

Animator::Animator() {
    count = 0;
}

Animator* Animator::add_animation(const char* texture_path, const std::string& name, int num_frames_per_row, int cell_width, int cell_height, float speed) {
    Animation anim = Animation();
    anim.texture = LoadTexture(texture_path);
    anim.first = 0;
    anim.last = num_frames_per_row-1;
    anim.curr = anim.first;
    anim.speed = speed;
    anim.duration_left = speed;
    anim.name = name;
    
    anim.num_frames_per_row = num_frames_per_row;
    anim.cell_width = cell_width;
    anim.cell_height = cell_height;
    
    auto it = animations.find(name);
    if(it != animations.end()) {
        std::cout<<"\n\nWARNING: Animations with the name "<<"'"<<name<<"'"<<" already exists\n";
    }
    else {
        animations[name] = anim;
    }
    
    count++;
    if(count == 1) {
        current_anim = &animations[name];
    }
    
    return this;
}

void Animator::play(const std::string& name) {
    paused = false;
    if(animations.find(name) == animations.end()) {
        std::cout<<"\n\nWARNING: Couldn't play animation with the name "<<"'"<<name<<"'"<<"\n";
    }
    else {
        current_anim = &animations[name];
    }
    
}

void Animator::pause() {
    paused = true;
}

void Animator::start() {
    sr = gameobject->get_component<SpriteRenderer>();
    paused = false;
}

void Animator::update(float deltaTime) {
    if(!paused) {
        current_anim->update(deltaTime);
    }
}

void Animator::draw(int offset[2]) {
    if(sr != nullptr) {
        DrawTexturePro(
            current_anim->texture,
            current_anim->get_frame(current_anim->num_frames_per_row, current_anim->cell_width, current_anim->cell_height),
            Rectangle{
                sr->transform->position.x - offset[0]*sr->depth,
                sr->transform->position.y - offset[1]*sr->depth,
                current_anim->cell_width*sr->transform->scale.x,
                current_anim->cell_height*sr->transform->scale.y
            },
            sr->origin,
            sr->transform->rotation,
            sr->tint
        );
    }
}