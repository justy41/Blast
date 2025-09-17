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

class SceneManager;
class Scene;
class GameObject;

class Component {
private:
public:
    bool enabled = true;
    GameObject* gameobject = nullptr;
    
    Component() = default;
    virtual void start() {}
    virtual void update(float deltaTime) {}
    virtual void draw() {}
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
    
    virtual void draw() {
        for(auto& comp : components) {
            if(comp.second->enabled) {
                comp.second->draw();
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
    SceneManager* manager;
    
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
                obj.second->draw();
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
            
            current_scene_index = other_scene_index;
            scenes[current_scene_index]->start();
            can_switch = false;
        }
        
        get_current_scene()->update(deltaTime);
        get_current_scene()->late_update(deltaTime);
    }
};

// Increases or decreases a value until the end by an amount (usually with deltaTime)
inline float MoveTowards(float start, float end, float amount) {
    if(start < end) return std::fmin(start + amount, end);
    if(start > end) return std::fmax(start - amount, end);
    return end;
}