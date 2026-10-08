#pragma once
#include "common.h"

static enum CAM_INFO 
{
    POS,
    PAST_POS,
    FORWARD,
    UP,
    RIGHT,
    LEFT
};
static Vec2 Cam_Info_ECS[6] = {
    (Vec2){.x = 0, .y = 0}, // POS
    (Vec2){.x = 0, .y = 0}, // PAST_POS
    (Vec2){.x = 0, .y = 0}, // FORWARD
    (Vec2){.x = 0, .y = 0}, // UP
    (Vec2){.x = 0, .y = 0}, // RIGHT
    (Vec2){.x = 0, .y = 0}  // LEFT
};

typedef struct TrackedCam
{
        Vec2 past_pos;
        Vec2 pos;
        Vec2 up;
        Vec2 right;
        Vec2 forward;
        Vec2 left;
        Camera2D cam;
} TrackedCam;

static Camera2D init_cam(const int x, const int y) {
    Camera2D cam = {0};
    cam.target = (Vec2){.x = x, .y = y};
    cam.offset = (Vec2){.x = x, .y = y};
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
    return cam;
}

static void move_cam(Camera2D* cam, const float speed)
{
    //WSAD
    if (IsKeyDown(KEY_W)) cam->offset.y -= speed;
    if (IsKeyDown(KEY_S)) cam->offset.y += speed;
    if (IsKeyDown(KEY_A)) cam->offset.x -= speed;
    if (IsKeyDown(KEY_D)) cam->offset.x += speed;
}
static void rotate_cam(Camera2D* cam, const float speed)
{
    if (IsKeyDown(KEY_Q)) cam->rotation -= speed;
    if (IsKeyDown(KEY_E)) cam->rotation += speed;
}

static Vec2 get_move_norm() 
{
    Vec2 move = {0};
    if (IsKeyDown(KEY_W)) move.y -= 1;
    if (IsKeyDown(KEY_S)) move.y += 1;
    if (IsKeyDown(KEY_A)) move.x -= 1;
    if (IsKeyDown(KEY_D)) move.x += 1;
    return Vector2Normalize(move);
}
static Vec2 scale_norm(Vec2 move, const float scalar) {
    return Vector2Scale(move, scalar);
}
static Vec2 apply_move(Vec2 move, Vec2 pos) {
    return Vector2Add(pos, move);
}
static void update_cam(Camera2D* cam, float move_speed, float rotate_speed) 
{
    Vec2 move_norm = get_move_norm();
    Vec2 move = scale_norm(move_norm, move_speed);
    cam->offset = apply_move(move, cam->offset);
    rotate_cam(cam, rotate_speed);
    cam->target = apply_move(cam->target, move);
}

// 3D Version
#if GAME_D == 3
Vec3 Cam_Info_ECS[6] = {
    (Vec3){.x = 0, .y = 0, .z = 0}, // POS
    (Vec3){.x = 0, .y = 0, .z = 0}, // PAST_POS
    (Vec3){.x = 0, .y = 0, .z = 0}, // FORWARD
    (Vec3){.x = 0, .y = 0, .z = 0}, // UP
    (Vec3){.x = 0, .y = 0, .z = 0}, // RIGHT
    (Vec3){.x = 0, .y = 0, .z = 0}  // LEFT
};
typedef struct TrackedCam
{
        Vec3 past_pos;
        Vec3 pos;
        Vec3 up;
        Vec3 right;
        Vec3 forward;
        Vec3 left;
        Camera3D cam;
} TrackedCam;

Camera3D init_cam(const Vec3 pos, const Vec3 target, const Vec3 up) {
    Camera3D cam = {0};
    cam.target = target;
    cam.position = pos;
    cam.projection = CAMERA_PERSPECTIVE;
    cam.up = up;
    return cam;
}
Vec3 get_forward(const Camera3D cam) {
    return Vector3Subtract(cam.target, cam.position);
}

Vec3 get_right(const Camera3D cam) {
    return Vector3CrossProduct(get_forward(cam), cam.up);
}

void rotate_cam(Camera3D cam, const float speed)
{
    if (IsKeyDown(KEY_Q)) cam.rotation -= speed;
    if (IsKeyDown(KEY_E)) cam.rotation += speed;
}
Vec3 get_flat(Vec3 v)
{
    return (Vec3){v.x, .y = 0, .z = v.z};
}
Vec3 get_move_norm(void)
{
    const signed int fwd = (int)IsKeyDown(KEY_W) - (int)IsKeyDown(KEY_S);
    const signed int strafe = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
    return Vector3Normalize((Vec3){.x = strafe, .y = 0, .z = fwd});
}
Vec3 scale_norm(const Vec3 move_norm, const float scalar) {
    return Vector3Scale(move_norm, scalar);
}
Vec3 calc_move(const Vec3 move, const Vec3 pos) {
    return Vector3Add(pos, move);
}

// call as move(pos, scale_norm(get_move_norm(), move_speed))
void move(Vec3* pos, const Vec3 scaled_move_norm_vector) 
{
    // apply our movement vector in the same direction as our forward/target
    *pos = calc_move(*pos, scaled_move_norm_vector);
}
#endif

#if FUNCTIONAL && GAME_D == 2
Camera2D move_cam(Camera2D cam, const float speed)
{
    //WSAD
    if (IsKeyDown(KEY_W)) cam.offset.y -= speed;
    if (IsKeyDown(KEY_S)) cam.offset.y += speed;
    if (IsKeyDown(KEY_A)) cam.offset.x -= speed;
    if (IsKeyDown(KEY_D)) cam.offset.x += speed;
    return cam;
}
Camera2D rotate_cam(Camera2D cam, const float speed)
{
    if (IsKeyDown(KEY_Q)) cam.rotation -= speed;
    if (IsKeyDown(KEY_E)) cam.rotation += speed;
    return cam;
}

Camera2D update_cam(Camera2D cam, float move_speed) 
{
    Vec2 move_norm = get_move_norm();
    Vec2 move = scale_norm(move_norm, move_speed);
    cam.offset = apply_move(move, cam->offset);
    return cam;
}

#endif

Vec3 ZOOM_IN(Vec3 offset, float amount);
Vec3 ZOOM_IN(Vec3 offset, float amount)
{
    float dx, dy, dz;
    dx = -amount*offset.x;
    dy = -amount*offset.y;
    dz = -amount*offset.z;
    return (Vec3){offset.x + dx, offset.y + dy, offset.z + dz};
}
Vec3 ZOOM_OUT(Vec3 offset, float amount);
Vec3 ZOOM_OUT(Vec3 offset, float amount)
{
    return ZOOM_IN(offset, -amount);
}
