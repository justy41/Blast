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
    
    /// @brief Loads a LDtk map
    /// @param tilemap_path the path to the *.ldtk file
    /// @param fixed_tile_size the fixed tile size (it is necessary to be fixed)
    /// @param collision_layer_names a std::string vector of names of the collision layers from the ldtk project (e.g. {"Ground", "Snow"})
    LDtkWorldComponent(const char* tilemap_path, float fixed_tile_size, std::vector<std::string> collision_layer_names);
    void start() override;
    void update(float deltaTime) override;
    void draw(int offset[2]) override;
    
    // Draws the entire LDtk map
    void draw_ldtk_map(int offset[2]);
    // Draws only the named level
    void draw_ldtk_level(const char* level_name);
    // Draws a layer from all levels (exception is entities layer)
    void draw_ldtk_layer(const char* layer_name);
    // Toggle the LDtk map collision hitboxes if F10 key is pressed
    void draw_ldtk_collision_layers();
    Rectangle getLDtkEntity(std::string Name_field);
    
    std::vector<Vector2> tiles_around(Vector2 pos, float tile_size, std::unordered_map<std::pair<float, float>, bool, FloatPairHash> collisions_layer);
    std::vector<Rectangle> physics_rects_around(Vector2 pos);
};