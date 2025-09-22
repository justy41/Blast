#pragma once
#include <iostream>
#include <cmath>
#include <string>

#include "raylib.h"
#include "engine.h"
#include "components/transform_component.h"
#include "LDtkLoader/Project.hpp"

class LDtkWorldComponent : public Component {
private:
    ldtk::IntRect r;
    std::unordered_map<std::string, Texture2D> tilesets;
    bool ldtk_debug;
public:
    ldtk::Project ldtkProject;
    const ldtk::World* world;
    float tile_size = 0;
    std::unordered_map<std::pair<float, float>, bool, FloatPairHash> collisions_layer;
    std::unordered_map<std::string, Rectangle> entities;
    
    LDtkWorldComponent(const char* tilemap_path, float fixed_tile_size, std::vector<std::string> collision_layer_names);
    void start() override;
    void update(float deltaTime) override;
    void draw() override;
    
    // Draws the entire LDtk map
    void draw_ldtk_map();
    // Draws only the named level
    void draw_ldtk_level(const char* level_name);
    // Draws a layer from all levels (exception is entities layer)
    void draw_ldtk_layer(const char* layer_name);
    // Toggle the LDtk map collision hitboxes if F6 key is pressed
    void draw_ldtk_collision_layers();
};