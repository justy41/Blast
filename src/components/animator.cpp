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