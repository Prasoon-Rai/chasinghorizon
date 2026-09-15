#include "raylib.h"
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "raylib-aseprite.h"


int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Running Simulator");
    SetTargetFPS(60);

    // World Constants
    Vector2 velocity = {0.0f, 0.0f};
    float gravity = 2000.0f;
    float jumpForce = 700.0f;
    bool isGrounded = true;

    // Player Character
    const Aseprite walking = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/walk.aseprite");
    AsepriteTag walkAnim = LoadAsepriteTagFromIndex(walking, 1);
    // AsepriteTag runAnim = LoadAsepriteTagFromIndex(walking, 0);

    // Player Properties
    Vector2 playerPos = {10.0f, 250.0f};
    float playerScale = 2.7f;
    float playerSpeed = 2.5f;

    // World Sprites
    const Image floor_img = LoadImage("/Users/prasoonrai/CLionProjects/RunninSim/assets/floor.png");
    Texture2D floor = LoadTextureFromImage(floor_img);
    UnloadImage(floor_img);
    SetTextureWrap(floor, TEXTURE_WRAP_REPEAT);

    // Ground Attributes
    float groundLevel = 250.0;
    Vector2 groundPos = {0.f, 370.0f};
    float groundScale = 1.5f;

    // Camera Setup
    Camera2D camera = { 0 };
    camera.target = (Vector2){ playerPos.x, groundPos.y };
    camera.offset = (Vector2){ playerPos.x, playerPos.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdateAsepriteTag(&walkAnim);

        camera.target = (Vector2) { playerPos.x, 290.0 };

        BeginDrawing();
        playerPos.x += playerSpeed;
        {
            ClearBackground(RAYWHITE);

            // Gravity Mechanics
            if (IsKeyPressed(KEY_SPACE) && isGrounded) {
                velocity.y = -jumpForce;
                isGrounded = false;
            }
            if (!isGrounded) {
                velocity.y += gravity * dt;
            } else {
                velocity.y = 0.0f;
            }
            playerPos.y += velocity.y * dt;
            if (playerPos.y >= groundLevel) {
                playerPos.y = groundLevel;
                isGrounded = true;
                velocity.y = 0.0f;
            }

            BeginMode2D(camera);
            DrawTextureEx(floor, groundPos, 0.0, 1.5, WHITE);
            DrawAsepriteTagExFlipped(walkAnim, playerPos,0.0f, playerScale, true, false, WHITE);
            EndMode2D();
        }
        EndDrawing();
    }

    UnloadAseprite(walking);
    UnloadTexture(floor);
    CloseWindow();
    return 0;
}