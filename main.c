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

    // Player Animations
    const Aseprite running = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/player/run.aseprite");
    AsepriteTag running_anim = LoadAsepriteTagFromIndex(running, 0);

    const Aseprite rolling = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/player/roll.aseprite");
    AsepriteTag rolling_anim = LoadAsepriteTagFromIndex(rolling, 0);

    const Aseprite jumping = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/player/jump.aseprite");
    AsepriteTag jumping_anim = LoadAsepriteTagFromIndex(jumping, 0);

    // Player Properties
    Vector2 playerPos = {10.0f, 250.0f};
    float playerScale = 2.5f;
    float playerSpeed = 500.0f;
    float jumpForce = 900.0f;
    bool isGrounded = true;
    bool isRolling = false;
    float rollTimer = 0.0f;
    const float roll_duration = 0.42f;

    // Mob Sprites
    const Aseprite flying_obstacle = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/obstacles/flying.aseprite");
    AsepriteTag flying_obstacle_anim = LoadAsepriteTagFromIndex(flying_obstacle, 0);

    const Aseprite ground_obstacle = LoadAseprite("/Users/prasoonrai/CLionProjects/RunninSim/assets/obstacles/flying.aseprite");
    AsepriteTag ground_obstacle_anim = LoadAsepriteTagFromIndex(ground_obstacle, 0);

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
    camera.offset = (Vector2){ playerPos.x, 250.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Background Music
    const Music backgroundSound = LoadMusicStream("/Users/prasoonrai/CLionProjects/RunninSim/assets/audio/background_music.mp3");
    SetMusicVolume(backgroundSound, 0.5f);
    //PlayMusicStream(backgroundSound);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdateMusicStream(backgroundSound);

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
        playerPos.x += playerSpeed * dt;

        ClearBackground(BACKGROUND_BLUE);

        // Drawing a constant cloud
        DrawTextureEx(cloud, cloudPos, 0.0f, cloudScale, WHITE);
        // DrawTextureEx(pillar, pillarPos, 0.0f, pillarScale, WHITE);
        {
            // Jumping Mechanics
            if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && isGrounded && !isRolling) {
                velocity.y = -jumpForce;
                isGrounded = false;
            }
            if (!isGrounded) {
                UpdateAsepriteTag(&jumping_anim);
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

            // Rolling Mechanics
            if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && isGrounded && !isRolling) {
                isRolling = true;
                rollTimer = roll_duration;
            }
            if (isRolling) {
                UpdateAsepriteTag(&rolling_anim);
                rollTimer -= dt;
                if (rollTimer <= 0.0f) {
                    isRolling = false;
                }
            } else {
                UpdateAsepriteTag(&running_anim);
            }

            if (IsKeyDown(KEY_DOWN)) {

            }

            BeginMode2D(camera);
            DrawTexturePro(pillar, sourceRecPillar, destRectPillar, pillarPos, 0.0f, WHITE);
            DrawTexturePro(floor, sourceRecFloor, destRecFloor, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
            if (isRolling) {
                DrawAsepriteTagExFlipped(rolling_anim, playerPos, 0.0f, playerScale, true, false, WHITE);
            }
            else if (!isGrounded) {
                float t = (velocity.y + jumpForce) / (2.0f * jumpForce);
                if (t < 0.0f) t = 0.0f;
                if (t > 1.0f) t = 1.0f;

                int frame = 4 + (int)(t * 43);   // frames 4..47 = rise, apex, fall
                DrawAsepriteExFlipped(jumping, frame, playerPos, 0.0f, playerScale, true, false, WHITE);
            }
            else {
                DrawAsepriteTagExFlipped(running_anim, playerPos, 0.0f, playerScale, true, false, WHITE);
            }
            EndMode2D();
        }
        EndDrawing();
    }

    UnloadAseprite(running);
    UnloadTexture(floor);
    UnloadTexture(cloud);
    UnloadTexture(pillar);
    UnloadMusicStream(backgroundSound);
    UnloadAseprite(rolling);
    UnloadAseprite(jumping);
    UnloadAseprite(flying_obstacle);
    CloseWindow();
    return 0;
}