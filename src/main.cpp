#include <iostream>
#include <cmath>
#include <string>

#include "rlImGui.h"
#include "imgui.h"
#include "raylib.h"
#include "engine.h"

#include "scenes/start_scene.h"
#include "scenes/next_scene.h"

inline void DrawLetterBox(RenderTexture2D target, float scale, int gameW, int gameH) {
    BeginDrawing();
        ClearBackground(BLACK);
        
        DrawTexturePro(target.texture, Rectangle{ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
            Rectangle{ (GetScreenWidth() - ((float)gameW*scale))*0.5f, (GetScreenHeight() - ((float)gameH*scale))*0.5f,
            (float)gameW*scale, (float)gameH*scale }, Vector2{ 0, 0 }, 0.0f, WHITE);
        
    EndDrawing();
}

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenW = 1280;
    const int screenH = 720;
    int gameW = 640;
    int gameH = 360;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenW, screenH, "game");

    RenderTexture2D target = LoadRenderTexture(gameW, gameH);
    float scale = 0;
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    // Camera
    Camera2D camera = { 0 };
    camera.target = Vector2{0, 0};
    camera.offset = Vector2{(float)gameW/2.f, (float)gameH/2.f};
    //camera.offset = Vector2{0, 0};
    camera.rotation = 0;
    camera.zoom = 1.0f;
    
    // ############################################################################################################################### //
    SceneManager sm = SceneManager();
    
    StartScene start_scene = StartScene();
    sm.add(&start_scene);
    
    NextScene next_scene = NextScene();
    sm.add(&next_scene);
    
    sm.get_current_scene()->start();
    // ############################################################################################################################### //
    
    rlImGuiSetup(true);
    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        scale = std::min((float)GetScreenWidth()/gameW, (float)GetScreenHeight()/gameH);
        sm.update(GetFrameTime());
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(target);
            ClearBackground(BLACK);
        
            BeginMode2D(camera);
        
                ClearBackground(RAYWHITE);
                sm.get_current_scene()->draw();
                
            EndMode2D();
            
        EndTextureMode();
        
        DrawLetterBox(target, scale, gameW, gameH);
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlImGuiShutdown();
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
