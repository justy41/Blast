#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "raylib.h"

inline int GAME_WIDTH;
inline int GAME_HEIGHT;

struct FloatPairHash {
    std::size_t operator()(const std::pair<float, float>& p) const {
        std::size_t h1 = std::hash<float>{}(p.first);
        std::size_t h2 = std::hash<float>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

inline std::vector<Vector2> NEIGHBOUR_OFFSETS = {
    Vector2{-1, 1},
    Vector2{0, 1},
    Vector2{1, 1},
    Vector2{-1, 0},
    Vector2{0, 0},
    Vector2{1, 0},
    Vector2{-1, -1},
    Vector2{0, -1},
    Vector2{1, -1}
};

// Increases or decreases a value until the end by an amount (usually with deltaTime)
inline float MoveTowards(float start, float end, float amount) {
    if(start < end) return std::fmin(start + amount, end);
    if(start > end) return std::fmax(start - amount, end);
    return end;
}

class SceneManager;
class Scene;
class GameObject;

class BlastCamera {
private:
public:
    float scroll[2] = {0, 0};
    int render_scroll[2] = {0, 0};
    float target_x = 0, target_y = 0;
    float damp = 1;
    
    float left = 0, right = 0, top = 0, bottom = 0;
    float min_x = -99999, max_x = 99999, min_y = -99999, max_y = 99999;
    
    BlastCamera() = default;
    void set_target(float x, float y, float damp) {
        target_x = x;
        target_y = y;
        this->damp = damp;
        
        left = target_x - GAME_WIDTH/2;
        right = target_x + GAME_WIDTH/2;
        top = target_y - GAME_HEIGHT/2;
        bottom = target_y + GAME_HEIGHT/2;
    }
    void confine(float min_x, float max_x, float min_y, float max_y) {
        this->min_x = min_x;
        this->max_x = max_x;
        this->min_y = min_y;
        this->max_y = max_y;
    }
    void reset() {
        scroll[0] = 0; scroll[1] = 0;
        render_scroll[0] = 0; render_scroll[1] = 0;
        target_x = 0; target_y = 0;
        damp = 1;
        left = 0; right = 0; top = 0; bottom = 0;
        min_x = -99999; max_x = 99999; min_y = -99999; max_y = 99999;
    }
    virtual ~BlastCamera() = default;
};

class Component {
private:
public:
    bool enabled = true;
    GameObject* gameobject = nullptr;
    
    Component() = default;
    virtual void start() {}
    virtual void update(float deltaTime) {}
    virtual void draw(int offset[2]) {}
    virtual void on_collision_enter_2d(GameObject* other) {}
    virtual ~Component() = default;
};

class GameObject {
private:
public:
    std::string name = "";
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    int type_count = 0;
    bool active = true;
    
    Scene* scene;
    
    GameObject(const char* name) {
        this->name = name;
    }
    
    virtual void start() {
        for(auto& comp : components) {
            if(comp.second->enabled) {
                comp.second->start();
            }
        }
    }
    
    virtual void update(float deltaTime) {
        for(auto& comp : components) {
            if(comp.second->enabled) {
                comp.second->update(deltaTime);
            }
        }
    }
    
    virtual void draw(int offset[2]) {
        for(auto& comp : components) {
            if(comp.second->enabled) {
                comp.second->draw(offset);
            }
        }
    }
    
    template<typename T>
    void add_component(T* component) {
        std::type_index ti(typeid(T));
        if(components.find(ti) == components.end()) {
            component->gameobject = this;
            components[ti] = std::unique_ptr<Component>(component);
        }
        else {
            std::cout<<"\n\nERROR: GameObject already has a component of this type!\n";
            std::exit(1);
        }
    }
    
    template<typename T>
    T* get_component() {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if(it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        else {
            return nullptr;
        }
    }
    
    template<typename T>
    bool has_component() {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if(it != components.end()) {
            return true;
        }
        else {
            return false;
        }
    }
    
    template<typename T>
    void remove_component() {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if(it != components.end()) {
            components.erase(ti);
        }
        else {
            std::cout<<"\n\nWARNING: trying to remove component that doesnt exist!\n";
        }
    }
};

class Scene {
private:
public:
    std::unordered_map<std::string, std::unique_ptr<GameObject>> gameobjects;
    std::unordered_map<std::string, GameObject*> gameobjects_to_remove;
    int index = 0;
    bool debug_mode = false;
    SceneManager* manager;
    
    // Camera stuff
    BlastCamera camera;
    
    Scene() {
        
    }
    
    // Creates a game object with a name and adds it to the scene
    // .
    // .
    // Note: if a game object is added and the name it's already in the scene,
    // then a number suffix is added (i.e. obj_name -> obj_name1)
    GameObject* create_gameobject(const char* name) {
        std::string n = name;
        if(gameobjects.find(n) == gameobjects.end()) {
            gameobjects[n] = std::make_unique<GameObject>(name);
            gameobjects[n]->scene = this;
        }
        else { // if the names collides then add a 
            n += std::to_string(++gameobjects[n]->type_count);
            gameobjects[n] = std::make_unique<GameObject>(n.c_str());
            gameobjects[n]->scene = this;
        }
        
        return gameobjects[n].get();
    }
    
    GameObject* find_gameobject_by_name(const char* name) {
        std::string n = name;
        if(gameobjects.find(n) != gameobjects.end()) {
            return gameobjects[n].get();
        }
        else {
            std::cout<<"\n\nERROR: could not find game object!\n";
            std::exit(1);
        }
    }
    
    void remove_gameobject(const char* name) {
        std::string n = name;
        if(gameobjects.find(n) != gameobjects.end()) {
            gameobjects_to_remove[n] = gameobjects[n].get();
        }
        else {
            std::cout<<"\n\nERROR: could not find game object to remove!\n";
            std::exit(1);
        }
    }
    
    void remove_gameobject(GameObject* gameobject) {
        if(gameobjects.find(gameobject->name) != gameobjects.end()) {
            gameobjects_to_remove[gameobject->name] = gameobject;
        }
        else {
            std::cout<<"\n\nERROR: could not find game object to remove!\n";
            std::exit(1);
        }
    }
    
    virtual void start() {
        for(auto& obj : gameobjects) {
            if(obj.second->active) {
                obj.second->start();
            }
        }
    }
    
    virtual void update(float deltaTime) {
        for(auto& obj : gameobjects) {
            if(obj.second->active) {
                obj.second->update(deltaTime);
            }
        }
        
        // Apply the scroll based on the target
        camera.scroll[0] += (camera.target_x - GAME_WIDTH/2 - camera.scroll[0])/camera.damp;
        camera.scroll[1] += (camera.target_y - GAME_HEIGHT/2 - camera.scroll[1])/camera.damp;
        
        // Clamp the scroll to the confine area
        float min_scroll_x = camera.min_x;
        float max_scroll_x = camera.max_x - GAME_WIDTH;
        float min_scroll_y = camera.min_y;
        float max_scroll_y = camera.max_y - GAME_HEIGHT;

        if (camera.scroll[0] < min_scroll_x) camera.scroll[0] = min_scroll_x;
        if (camera.scroll[0] > max_scroll_x) camera.scroll[0] = max_scroll_x;
        if (camera.scroll[1] < min_scroll_y) camera.scroll[1] = min_scroll_y;
        if (camera.scroll[1] > max_scroll_y) camera.scroll[1] = max_scroll_y;
        
        camera.render_scroll[0] = (int)camera.scroll[0];
        camera.render_scroll[1] = (int)camera.scroll[1];
    }
    
    virtual void late_update(float deltaTime) {
        
        // After everything is updated, remove all game objects queued for deletion
        if(!gameobjects_to_remove.empty()) {
            for(auto obj : gameobjects_to_remove) {
                gameobjects.erase(obj.first);
            }
            gameobjects_to_remove.clear(); // And after that clear the queue
        }
    }
    
    virtual void draw() {
        for(auto& obj : gameobjects) {
            if(obj.second->active) {
                obj.second->draw(camera.render_scroll);
            }
        }
    }
};

class SceneManager {
private:

public:
    
    std::vector<Scene*> scenes;
    int current_scene_index = 0;
    int other_scene_index = 0;
    bool can_switch = false;
    
    SceneManager() = default;
    
    void add(Scene* scene) {
        scene->manager = this;
        scenes.push_back(scene);
        scene->index = (int)scenes.size() - 1;
    }
    
    Scene* get_current_scene() {
        return scenes[current_scene_index];
    }
    
    void switch_scene(int other_scene_index) {
        this->other_scene_index = other_scene_index;
        can_switch = true;
    }
    
    void update(float deltaTime) {
        if(can_switch) {
            scenes[current_scene_index]->gameobjects.clear();
            scenes[current_scene_index]->gameobjects_to_remove.clear();
            scenes[current_scene_index]->camera.reset();
            
            current_scene_index = other_scene_index;
            scenes[current_scene_index]->start();
            can_switch = false;
        }
        
        get_current_scene()->update(deltaTime);
        get_current_scene()->late_update(deltaTime);
    }
};