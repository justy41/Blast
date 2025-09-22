#include "components/ldtk_world_component.h"

LDtkWorldComponent::LDtkWorldComponent(const char* tilemap_path, float fixed_tile_size, std::vector<std::string> collision_layer_names) {
    ldtkProject.loadFromFile(tilemap_path);
    world = &ldtkProject.getWorld();
    
    tile_size = fixed_tile_size;
    for(const auto& level : world->allLevels()) {
        if(!collision_layer_names.empty()) {
            for(auto name : collision_layer_names) {
                for(const auto& tile : level.getLayer(name).allTiles()) {
                    collisions_layer.insert({std::make_pair(tile.getGridPosition().x + level.position.x/tile_size, tile.getGridPosition().y + level.position.y/tile_size), true});
                }
            }
        }
        else break;
    }
    
    // This block of code takes the tilemap_path and erases the map.ldtk part.
    // After that, the tileset path from the ldtk layer is appended to it, forming the path to the tileset.
    // EXAMPLE: tilemaps/map.ldtk + ../tilesets/tileset_1.png = tilemaps/../tilesets/tileset_1.png
    //
    // The entities are also extracted in an unordered_map for later use.
    for(const auto& level : world->allLevels()) {
        for(const auto& layer : level.allLayers()) {
            if(layer.getType() != ldtk::LayerType::Entities) {
                if(tilesets.find(layer.getTileset().name) == tilesets.end()) {
                    std::string texture_file_name = layer.getTileset().path; // Load file path relative to the .ldtk file
                    std::string map_path = tilemap_path;
                    for(int i = map_path.size()-1; i>=0; i--) {
                        if(map_path[i-1] == '/') {      // When the '/' is next...
                            map_path.erase(i);          // ...then erase.
                            break;
                        }
                    }
                    
                    map_path.append(texture_file_name); // Combine the two file paths.
                    tilesets.insert({layer.getTileset().name, LoadTexture(map_path.c_str())}); // Insert the name and load the tileset.
                    std::cout<<"\nTILESET PATH:\n"<<map_path<<"\n\n";
                }
            }
            else {
                // Saving ldtk entities as an element with Name, Position and Size in an unordered_map
                for(const auto& ent : layer.allEntities()) {
                    entities.insert({
                        ent.getField<std::string>("Name").value(),
                        Rectangle{
                            (float)ent.getPosition().x + level.position.x,
                            (float)ent.getPosition().y + level.position.y,
                            (float)ent.getSize().x,
                            (float)ent.getSize().y
                        }
                    });
                }
            }
        }
    }
}

void LDtkWorldComponent::start() {
    
}

void LDtkWorldComponent::update(float deltaTime) {
    
}

void LDtkWorldComponent::draw() {
    
}

void LDtkWorldComponent::draw_ldtk_map() {
    for(const auto& level : world->allLevels()) {
        for(int i = level.allLayers().size()-1; i>=0; i--) { // Reversed order because LDtkLoader takes the layers inverted.
            if(level.allLayers()[i].getType() != ldtk::LayerType::Entities) {
                std::string ts_name = level.allLayers()[i].getTileset().name; // Get the tileset name of the current layer.
                if(level.allLayers()[i].isVisible()) {
                    for(const auto& tile : level.allLayers()[i].allTiles()) { // Draw all tiles based on their source rect.
                        r = tile.getTextureRect(); // source rect
                        DrawTextureRec(
                            tilesets[ts_name],
                            Rectangle{(float)r.x, (float)r.y, (float)r.width, (float)r.height},
                            Vector2{(float)tile.getPosition().x + level.position.x, (float)tile.getPosition().y + level.position.y},
                            WHITE
                        );
                    }
                }
            }
        }
    }
}

void LDtkWorldComponent::draw_ldtk_level(const char* level_name) {
    for(int i = world->getLevel(level_name).allLayers().size()-1; i>=0; i--) {
        if(world->getLevel(level_name).allLayers()[i].getType() != ldtk::LayerType::Entities) {
            std::string ts_name = world->getLevel(level_name).allLayers()[i].getTileset().name;
            if(world->getLevel(level_name).allLayers()[i].isVisible()) {
                for(const auto& tile : world->getLevel(level_name).allLayers()[i].allTiles()) {
                    r = tile.getTextureRect();
                    DrawTextureRec(
                        tilesets[ts_name],
                        Rectangle{(float)r.x, (float)r.y, (float)r.width, (float)r.height},
                        Vector2{(float)tile.getPosition().x, (float)tile.getPosition().y},
                        WHITE
                    );
                }
            }
        }
    }
}

void LDtkWorldComponent::draw_ldtk_layer(const char* layer_name) {
    for(const auto& level : world->allLevels()) {
        std::string ts_name = level.getLayer(layer_name).getTileset().name;
        if(level.getLayer(layer_name).isVisible()) {
            for(const auto& tile : level.getLayer(layer_name).allTiles()) {
                r = tile.getTextureRect();
                DrawTextureRec(
                    tilesets[ts_name],
                    Rectangle{(float)r.x, (float)r.y, (float)r.width, (float)r.height},
                    Vector2{(float)tile.getPosition().x + level.position.x, (float)tile.getPosition().y + level.position.y},
                    WHITE
                );
            }
        }
    }
}

void LDtkWorldComponent::draw_ldtk_collision_layers() {
    if(IsKeyPressed(KEY_F10)) {
        ldtk_debug = !ldtk_debug;
    }
    
    if(ldtk_debug && !collisions_layer.empty()) {
        for(auto tile : collisions_layer) {
            DrawRectangleLinesEx(Rectangle{tile.first.first*tile_size, tile.first.second*tile_size, tile_size, tile_size}, 2, RED);
        }
    }
}