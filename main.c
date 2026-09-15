#include "raylib.h"
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "raylib-aseprite.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Running Simulator");
    SetTargetFPS(60);

    const Aseprite walking = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/walk.aseprite");

    // Player Character
    AsepriteTag walkAnim = LoadAsepriteTagFromIndex(walking, 1);
    // AsepriteTag runAnim = LoadAsepriteTagFromIndex(walking, 0);

    while (!WindowShouldClose()) {
        // Player Properties
        Vector2 playerPos = {100.0f, 100.0f};
        float playerScale = 2.0f;

        UpdateAsepriteTag(&walkAnim);
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawAsepriteTagExFlipped(walkAnim, playerPos,0.0f, playerScale, true, false, WHITE);
        }
        EndDrawing();
    }

    UnloadAseprite(walking);
    CloseWindow();
    return 0;
}