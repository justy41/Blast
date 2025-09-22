#include "components/box_collider2d.h"

BoxCollider2D::BoxCollider2D(float width, float height, float rel_x, float rel_y) {
    this->width = width;
    this->height = height;
    rel_position = Vector2{rel_x, rel_y};
}

void BoxCollider2D::start() {
    transform = gameobject->get_component<TransformComponent>();
    
    if(transform != nullptr) {
        rect = Rectangle{
            transform->position.x + rel_position.x,
            transform->position.y + rel_position.y,
            width,
            height
        };
    }
    else {
        rect = Rectangle{0, 0, 0, 0};
    }
}

void BoxCollider2D::update(float deltaTime) {
    rect.x = transform->position.x + rel_position.x;
    rect.y = transform->position.y + rel_position.y;
    
    for(auto& [name, obj] : gameobject->scene->gameobjects) {
        if(obj != nullptr && gameobject != obj.get() && obj->has_component<BoxCollider2D>()) {
            if(CheckCollisionRecs(rect, obj->get_component<BoxCollider2D>()->rect)) {
                for(auto& [_, comp] : gameobject->components) {
                    comp->on_collision_enter_2d(obj.get());
                }
                for(auto& [_, comp] : obj->components) {
                    comp->on_collision_enter_2d(gameobject);
                }
            }
        }
    }
    
    if(IsKeyPressed(KEY_F10)) gameobject->scene->debug_mode = !gameobject->scene->debug_mode;
}

void BoxCollider2D::draw() {
    if(rect.width != 0 && gameobject->scene->debug_mode) {
        DrawRectangleLinesEx(rect, 2, GREEN);
    }
}