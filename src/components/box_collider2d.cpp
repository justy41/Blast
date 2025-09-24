#include "components/box_collider2d.h"

BoxCollider2D::BoxCollider2D(float width, float height, float rel_x, float rel_y) {
    this->width = width;
    this->height = height;
    rel_position = Vector2{rel_x, rel_y};
    ldtk_world = nullptr;
    collisions.insert({{"right", false}, {"left", false}, {"down", false}, {"up", false}});
}

void BoxCollider2D::start() {
    transform = gameobject->get_component<TransformComponent>();
    rb = gameobject->get_component<Rigidbody2D>();
    
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
    // Setting them to false every frame
    collisions["right"] = false;
    collisions["left"] = false;
    collisions["down"] = false;
    collisions["up"] = false;
    
    if(transform != nullptr && rb != nullptr) {
        transform->position.x += rb->velocity.x * deltaTime;
        // ================================================ TILEMAP COLLISION RESOLUTION X ================================================ //
        rect.x = transform->position.x + rel_position.x;
        if(ldtk_world != nullptr) {
            for(Rectangle r : ldtk_world->physics_rects_around(transform->position)) {
                if(CheckCollisionRecs(rect, r)) {
                    if(rb->velocity.x > 0) {
                        rect.x = r.x - rect.width;
                        collisions["right"] = true;
                    }
                    if(rb->velocity.x < 0) {
                        rect.x = r.x + r.width;
                        collisions["left"] = true;
                    }
                    transform->position.x = rect.x - rel_position.x;
                }
            }
            
            if(collisions["right"] || collisions["left"]) {
                rb->velocity.x = 0;
            }
        }
        // ================================================================================================================================ //
        
        rb->velocity.y += rb->gravity * deltaTime;
        transform->position.y += rb->velocity.y * deltaTime;
        // ================================================ TILEMAP COLLISION RESOLUTION Y ================================================ //
        rect.y = transform->position.y + rel_position.y;
        if(ldtk_world != nullptr) {
            for(Rectangle r : ldtk_world->physics_rects_around(transform->position)) {
                if(CheckCollisionRecs(rect, r)) {
                    if(rb->velocity.y > 0) {
                        rect.y = r.y - rect.height;
                        collisions["down"] = true;
                    }
                    if(rb->velocity.y < 0) {
                        rect.y = r.y + r.height;
                        collisions["up"] = true;
                    }
                    transform->position.y = rect.y - rel_position.y;
                }
            }
            
            if(collisions["down"] || collisions["up"]) {
                rb->velocity.y = 0;
            }
        }
        // ================================================================================================================================ //
    }
    
    // Checking for collisions between colliders
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
    
    // The collision checking with the tilemap needs to be separate so the check wont perform twice (because there are two axis)
    if(ldtk_world != nullptr) {
        if(collisions["right"] || collisions["left"] || collisions["down"] || collisions["up"]) {
            for(auto& [_, comp] : gameobject->components) {
                comp->on_collision_enter_2d(ldtk_world->gameobject);
            }
        }
    }
}

void BoxCollider2D::draw(int offset[2]) {
    if(rect.width != 0 && gameobject->scene->debug_mode) {
        DrawRectangleLinesEx(rect, 2, GREEN);
    }
}