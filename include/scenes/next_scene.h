#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <memory>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "components/sprite_renderer.h"

class NextScene : public Scene {
private:
public:
    GameObject* player;
    
    // Runs once at the start of the scene
    // Note: initilize all the variables in here
    void start() override {
        player = create_gameobject("player");
        player->add_component(new TransformComponent(100, 100, 0, 1, 1));
        player->add_component(new SpriteRenderer(RESOURCES_PATH "cat.png"));
        
        Scene::start();
    }
    
    // Runs every frame
    void update(float deltaTime) override {
        Scene::update(deltaTime);
        
        if(IsKeyPressed(KEY_ENTER)) {
            manager->switch_scene(0);
        }
    }
    
    // Runs every frame
    void draw() override {
        ClearBackground(BLUE);
        Scene::draw();
    }
};