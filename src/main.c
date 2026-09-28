#include "common.h"
#include "camera.h"
#include "file_directory_managment.h"
#include "structures.h"

// Must be 2 or 3.
#define GAME_D 2
#ifndef GAME_D
#define GAME_D 2
#endif
#if (GAME_D < 2) || (GAME_D > 3)
#define GAME_D 2
#endif

#define VIRT_WIDTH 600
#define VIRT_HEIGHT 600
#define DEFAULT_FPS 60
#define BG_COLOR BLACK

#if GAME_D == 2
int main(void)
{
        int SCREEN_W = VIRT_WIDTH;
        int SCREEN_H = VIRT_HEIGHT;
        int TARGET_FPS = DEFAULT_FPS;

        SetWindowState(FLAG_WINDOW_RESIZABLE);
        InitWindow(SCREEN_W, SCREEN_H, "Raylib_Template");
        SetTargetFPS(TARGET_FPS);

        Cam2D cam = cam2d_make((Vec2){0});
        const RenderTexture2D renderTexture = LoadRenderTexture(VIRT_WIDTH, VIRT_HEIGHT);
        const Rectangle renderTextureSrc = {0, 0, VIRT_WIDTH, -VIRT_HEIGHT};
        const Vec2 renderTextureOrigin = (Vec2){0};

        while (!WindowShouldClose())
        {
                float dt = GetFrameTime();
                CamInput in = cam_input_read();

                cam2d_update(&cam, in, dt);

                int W = GetScreenWidth();
                int H = GetScreenHeight();
                Rectangle renderTextureDst = 
                {
                        .x = 0,
                        .y = 0,
                        .width = W,
                        .height = H
                };
                // Pass 1: Render world using VIRTUAL coordinates
                BeginTextureMode(renderTexture);
                        ClearBackground(BG_COLOR);
                        BeginMode2D(cam.base_cam);
                                // Always use VIRT_WIDTH and VIRT_HEIGHT here
                                DrawRectangle(VIRT_WIDTH / 2, VIRT_HEIGHT / 2, 10, 10, RED);
                                DrawText("I am in the renderTexture's center", VIRT_WIDTH / 2, VIRT_HEIGHT / 2, 10, WHITE);
                        EndMode2D();
                EndTextureMode();

                // Pass 2: Draw virtual texture onto the actual window
                BeginDrawing();
                        ClearBackground(BG_COLOR);
                        DrawTexturePro(renderTexture.texture, renderTextureSrc, renderTextureDst, renderTextureOrigin, 0.0f, WHITE);
                EndDrawing();
        }

        UnloadRenderTexture(renderTexture);
        CloseWindow();
        return 0;
}
#endif

#if GAME_D == 3
int main(void)
{

        int SCREEN_W = VIRT_WIDTH;
        int SCREEN_H = VIRT_HEIGHT;
        int TARGET_FPS = DEFAULT_FPS;
        const char *RESOURCES_PATH = "./resources/";

        SetWindowState(FLAG_WINDOW_RESIZABLE);
        InitWindow(SCREEN_W, SCREEN_H, "Raylib_Template");
        SetTargetFPS(TARGET_FPS);


        Vec3 cam_pos = {.x = 0, .y = 1, .z = 0};
        Vec3 cam_target = {.x = 0, .y = 1, .z = -1};
        Vec3 cam_up = {.x = 0, .y = 1, .z = 0};
        float cam_fovy = 120.0f;
        float cam_mov_speed = 100.0f;
        float cam_turn_speed = 1.0f;
        float cam_pitch_limit = 0.0f;
        CamMoveMode cam_mode = CAM_MOVE_FLAT;
        Cam3D cam = cam3d_make(cam_pos, cam_target, cam_up, cam_fovy, cam_mov_speed, cam_turn_speed, cam_pitch_limit, cam_mode);


        while (!WindowShouldClose())
        {
                float dt = GetFrameTime();
                int H = GetScreenHeight();
                int W = GetScreenWidth();
                const float time_passed = GetTime();
                CamInput in = cam_input_read();

                cam3d_update(&cam, in, dt);
                
                BeginDrawing();
                ClearBackground(BG_COLOR);

                BeginMode3D(cam.base_cam);
                    DrawCube((Vec3){0}, 10, 10, 10, RED);
                EndMode3D();

                EndDrawing();
        }

        CloseWindow();
        return 0;
}
#endif