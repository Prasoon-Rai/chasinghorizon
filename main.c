#include "raylib.h"
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "raylib-aseprite.h"

#define BACKGROUND_BLUE (Color) {91, 139, 213, 255}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Chasing Horizon");
    InitAudioDevice();
    SetTargetFPS(60);

    // World Constants
    Vector2 velocity = {0.0f, 0.0f};
    float gravity = 2000.0f;
    float jumpForce = 700.0f;
    bool isGrounded = true;

    // Player Character
    const Aseprite walking = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/player/walk.aseprite");
    AsepriteTag walkAnim = LoadAsepriteTagFromIndex(walking, 1);
    // AsepriteTag runAnim = LoadAsepriteTagFromIndex(walking, 0);

    // Player Properties
    Vector2 playerPos = {10.0f, 250.0f};
    float playerScale = 2.7f;
    float playerSpeed = 2.5f;

    // World Sprites
    const Image floor_img = LoadImage("/Users/prasoonrai/CLionProjects/RunninSim/assets/world/floor.png");
    Texture2D floor = LoadTextureFromImage(floor_img);
    UnloadImage(floor_img);
    SetTextureWrap(floor, TEXTURE_WRAP_REPEAT);

    const Image cloud_img = LoadImage("/Users/prasoonrai/CLionProjects/RunninSim/assets/world/cloud2.png");
    Texture2D cloud = LoadTextureFromImage(cloud_img);
    UnloadImage(cloud_img);

    Image pillar_img = LoadImage("/Users/prasoonrai/CLionProjects/RunninSim/assets/world/pillar3.png");
    ImageAlphaCrop(&pillar_img, 0.9f);
    Texture2D pillar = LoadTextureFromImage(pillar_img);
    UnloadImage(pillar_img);
    SetTextureWrap(pillar, TEXTURE_WRAP_REPEAT);

    // Ground Attributes
    float groundLevel = 250.0;
    Vector2 groundPos = {0.f, 370.0f};
    float groundScale = 1.5f;

    // Cloud Attributes
    Vector2 cloudPos = {-40.0f, 95.0f};
    float cloudScale = 1.0f;

    // Pillar Attributes
    Vector2 pillarPos = { 0.0f, 0.0f };
    float pillarScale = 0.21f;

    // Camera Setup
    Camera2D camera = { 0 };
    camera.target = (Vector2){ playerPos.x, groundPos.y };
    camera.offset = (Vector2){ playerPos.x, playerPos.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Background Audio
    const Sound backgroundSound = LoadSound("/Users/prasoonrai/CLionProjects/RunninSim/assets/audio/background_music.mp3");


    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdateAsepriteTag(&walkAnim);

        camera.target = (Vector2) { playerPos.x, 290.0 };

        float floor_repeat_count = 100.0f;
        Rectangle sourceRecFloor = { 0.0f, 0.0f, floor.width * floor_repeat_count, floor.height };
        Rectangle destRecFloor = {
            groundPos.x,
            groundPos.y,
            (float)floor.width * groundScale * floor_repeat_count,
            (float)floor.height * groundScale
        };

        float pillar_repeat_count = 10000.0f;
        Rectangle sourceRecPillar = { 0.0f, 0.0f, pillar.width * pillar_repeat_count, pillar.height};
        Rectangle destRectPillar = {
            pillarPos.x,
            pillarPos.y,
            pillar.width * pillarScale * pillar_repeat_count,
            pillar.height * pillarScale
        };

        BeginDrawing();
        playerPos.x += playerSpeed;

        // Drawing a constant cloud
        DrawTextureEx(cloud, cloudPos, 0.0f, cloudScale, WHITE);
        // DrawTextureEx(pillar, pillarPos, 0.0f, pillarScale, WHITE);
        {
            ClearBackground(BACKGROUND_BLUE);

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
            DrawTexturePro(pillar, sourceRecPillar, destRectPillar, pillarPos, 0.0f, WHITE);
            DrawTexturePro(floor, sourceRecFloor, destRecFloor, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
            DrawAsepriteTagExFlipped(walkAnim, playerPos,0.0f, playerScale, true, false, WHITE);
            EndMode2D();
        }
        EndDrawing();
    }

    UnloadAseprite(walking);
    UnloadTexture(floor);
    UnloadTexture(cloud);
    UnloadTexture(pillar);
    UnloadSound(backgroundSound);
    CloseWindow();
    return 0;
}