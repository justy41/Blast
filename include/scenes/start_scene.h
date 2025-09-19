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

class StartScene : public Scene {
private:
public:
    GameObject* player;
    
    // Runs once at the start of the scene
    // Note: initilize all the variables in here
    void start() override {
        player = create_gameobject("player");
        player->add_component(new TransformComponent(0, 0, 0, 1, 1));
        player->add_component(new SpriteRenderer(RESOURCES_PATH "cat.png"));
        player->add_component(new Rigidbody2D(0, 0, 0));
        
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
    }
};