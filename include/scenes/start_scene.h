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
#include "components/animator.h"

// TODO: add a camera to be able to follow the player. Set the player as its target.
class StartScene : public Scene {
private:
public:
    GameObject* player; TransformComponent* player_transform;
    GameObject* other_object;
    GameObject* ldtk_world; LDtkWorldComponent* ldtk_world_component;
    
    // Runs once at the start of the scene
    // Note: initilize all the variables in here
    void start() override {
        player = create_gameobject("player");
        player->add_component(new TransformComponent(120, 50, 0, 1, 1));
        player->add_component(new BoxCollider2D(16, 16));
        player->add_component(new SpriteRenderer(RESOURCES_PATH "cat.png", 1, 8, 8));
        player->add_component(new Rigidbody2D(0, 0, 750));
        player->add_component(new PlayerMovement(180, 800, 250));
        player->add_component(new Animator());
        player->get_component<Animator>()
            ->add_animation(RESOURCES_PATH "cat.png", "idle", 1, 32, 32, 0.1, REPEATING)
            ->add_animation(RESOURCES_PATH "animations/player_run.png", "run", 5, 32, 32, 0.06, REPEATING)
            ->add_animation(RESOURCES_PATH "animations/player_jump.png", "jump", 6, 32, 32, 0.06, ONESHOT);
        player->get_component<Animator>()->play("idle");
        
        other_object = create_gameobject("other_object");
        other_object->add_component(new TransformComponent(200, 60, 0, 1, 1));
        other_object->add_component(new BoxCollider2D(16, 16));
        other_object->add_component(new SpriteRenderer(RESOURCES_PATH "bird.png", 0.6));
        
        ldtk_world = create_gameobject("ldtk_world");
        ldtk_world->add_component(new LDtkWorldComponent(RESOURCES_PATH "tilemaps/map_0.ldtk", 16, {"Ground", "Snow"}));
        ldtk_world_component = ldtk_world->get_component<LDtkWorldComponent>();
        
        
        
        player_transform = player->get_component<TransformComponent>();
        camera.set_target(
            player_transform->position.x,
            player_transform->position.y,
            1
        );
        camera.confine(0, 1280, 0, 736);
        
        Scene::start();
    }
    
    // Runs every frame
    void update(float deltaTime) override {
        Scene::update(deltaTime);
        
        camera.set_target(
            player_transform->position.x,
            player_transform->position.y,
            30
        );
        
        
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
        
        // Calling this function draws the LDtk map
        // ldtk_world->get_component<LDtkWorldComponent>()->draw_ldtk_map(camera.render_scroll);
        // ldtk_world_component->draw_ldtk_layer("Ground", camera.render_scroll);
        // ldtk_world_component->draw_ldtk_layer("Snow", camera.render_scroll);
        // ldtk_world_component->draw_ldtk_layer("Decour_ground", camera.render_scroll);
        // ldtk_world_component->draw_ldtk_layer("Decour_snow", camera.render_scroll);
        // ldtk_world_component->draw_ldtk_layer("Parallax", camera.render_scroll, 1.09);
        
        ldtk_world_component->draw_ldtk_layer("BG", camera.render_scroll);
        Scene::draw();
        ldtk_world_component->draw_ldtk_map_without_layers({"BG", "FG"}, camera.render_scroll);
        ldtk_world_component->draw_ldtk_layer("FG", camera.render_scroll);
        
        ldtk_world_component->draw_ldtk_collision_layers(camera.render_scroll); // Pressing F10 shows the collision layers colliders
    }
};