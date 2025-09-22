#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <memory>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "components/sprite_renderer.h"
#include "components/rigidbody2d.h"
#include "components/box_collider2d.h"
#include "game_scripts/player_movement.h"
#include "components/ldtk_world_component.h"

class StartScene : public Scene {
private:
public:
    GameObject* player;
    GameObject* other_object;
    GameObject* ldtk_world;
    
    // Runs once at the start of the scene
    // Note: initilize all the variables in here
    void start() override {
        player = create_gameobject("player");
        player->add_component(new TransformComponent(0, 0, 0, 1, 1));
        player->add_component(new BoxCollider2D(16, 16));
        player->add_component(new SpriteRenderer(RESOURCES_PATH "cat.png"));
        player->add_component(new Rigidbody2D(0, 0, 0));
        player->add_component(new PlayerMovement(180, 500));
        
        other_object = create_gameobject("other_object");
        other_object->add_component(new TransformComponent(-100, -100, 0, 1, 1));
        other_object->add_component(new BoxCollider2D(16, 16));
        other_object->add_component(new SpriteRenderer(RESOURCES_PATH "bird.png"));
        
        ldtk_world = create_gameobject("ldtk_world");
        ldtk_world->add_component(new LDtkWorldComponent(RESOURCES_PATH "tilemaps/map_0.ldtk", 16, {"Ground", "Snow"}));
        
        Scene::start();
    }
    
    // Runs every frame
    void update(float deltaTime) override {
        Scene::update(deltaTime);
        
        // This is how you can switch scenes
        if(IsKeyPressed(KEY_ENTER)) {
            manager->switch_scene(1);
        }
        
        // This is how you can remove components
        if(IsKeyPressed(KEY_R)) {
            player->remove_component<SpriteRenderer>();
        }
    }
    
    // Runs every frame
    void draw() override {
        ClearBackground(YELLOW);
        Scene::draw();
        ldtk_world->get_component<LDtkWorldComponent>()->draw_ldtk_map();
    }
};