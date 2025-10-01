#include "components/animator.h"
#include "components/sprite_renderer.h"

void Animation::update(float deltaTime) {
    duration_left -= deltaTime;
    
    if(duration_left <= 0) {
        duration_left = speed;
        curr++;
        
        if(curr > last) {
            switch(type) {
            case REPEATING:
                curr = first;
                break;
            case ONESHOT:
                curr = last;
                break;
            }
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

Animator* Animator::add_animation(const char* texture_path, const std::string& name, int num_frames_per_row, int cell_width, int cell_height, float speed, AnimationType type) {
    auto anim = std::make_unique<Animation>();
    anim->texture = LoadTexture(texture_path);
    anim->first = 0;
    anim->last = num_frames_per_row-1;
    anim->curr = anim->first;
    anim->speed = speed;
    anim->duration_left = speed;
    anim->name = name;
    anim->type = type;
    
    anim->num_frames_per_row = num_frames_per_row;
    anim->cell_width = cell_width;
    anim->cell_height = cell_height;
    
    auto it = animations.find(name);
    if(it != animations.end()) {
        std::cout<<"\n\nWARNING: Animations with the name "<<"'"<<name<<"'"<<" already exists\n";
    }
    else {
        animations[name] = std::move(anim);
    }
    
    count++;
    if(count == 1) {
        current_anim = animations[name].get();
    }
    
    return this;
}

void Animator::play(const std::string& name) {
    paused = false;
    auto it = animations.find(name);
    if(it == animations.end()) {
        std::cout<<"\n\nWARNING: Couldn't play animation with the name "<<"'"<<name<<"'"<<"\n";
        return;
    }
    
    if(current_anim == nullptr || current_anim->name != name) {
        current_anim = it->second.get();
        current_anim->curr = current_anim->first;
        current_anim->duration_left = current_anim->speed;
    }
}

void Animator::pause() {
    paused = true;
    exists = true;
}

void Animator::start() {
    sr = gameobject->get_component<SpriteRenderer>();
    paused = false;
    exists = true;
}

void Animator::update(float deltaTime) {
    if(!paused) {
        current_anim->update(deltaTime);
    }
}

void Animator::draw(int offset[2]) {
    if(sr == nullptr || current_anim == nullptr) {
        return;
    }
    
    DrawTexturePro(
        current_anim->texture,
        current_anim->get_frame(current_anim->num_frames_per_row, current_anim->cell_width, current_anim->cell_height),
        Rectangle{
            sr->transform->position.x - offset[0]*sr->depth,
            sr->transform->position.y - offset[1]*sr->depth,
            (float)current_anim->cell_width * sr->transform->scale.x,
            (float)current_anim->cell_height * sr->transform->scale.y
        },
        sr->origin,
        sr->transform->rotation,
        sr->tint
    );
}