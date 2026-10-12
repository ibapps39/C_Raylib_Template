#pragma once
#include "common.h"

// index into Cam_Info_ECS
typedef enum CAM_INFO
{
    POS,
    PAST_POS,
    FORWARD,
    UP,
    RIGHT
} CAM_INFO;

/* ------------------------------------------------------------------ */
/* 2D                                                                  */
/* ------------------------------------------------------------------ */
#if GAME_D == 2

static Vec2 Cam_Info_ECS[5] = {0};

typedef struct TrackedCam
{
    Vec2 past_pos;
    Vec2 pos;
    Vec2 up;
    Vec2 right;
    Vec2 forward;
    Camera2D cam;
} TrackedCam;

static Camera2D init_cam(const int offset_x, const int offset_y)
{
    Camera2D cam = {0};
    cam.target = (Vec2){.x = offset_x, .y = offset_y};
    cam.offset = (Vec2){.x = offset_x, .y = offset_y};
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
    return cam;
}

static Vec2 get_move_norm(void)
{
    Vec2 move = {0};
    if (IsKeyDown(KEY_W)) move.y -= 1;
    if (IsKeyDown(KEY_S)) move.y += 1;
    if (IsKeyDown(KEY_A)) move.x -= 1;
    if (IsKeyDown(KEY_D)) move.x += 1;
    return Vector2Normalize(move);
}
static Vec2 scale_norm(const Vec2 move_norm, const float scalar)
{
    return Vector2Scale(move_norm, scalar);
}
static Vec2 apply_move(const Vec2 move, const Vec2 pos)
{
    return Vector2Add(pos, move);
}

// WSAD, moves the camera through the world (target only, offset stays the screen anchor)
static void move_cam(Camera2D* cam, const float speed)
{
    const Vec2 move = scale_norm(get_move_norm(), speed);
    cam->target = apply_move(move, cam->target);
}
static void rotate_cam(Camera2D* cam, const float speed)
{
    if (IsKeyDown(KEY_Q)) cam->rotation -= speed;
    if (IsKeyDown(KEY_E)) cam->rotation += speed;
}
static void update_cam(Camera2D* cam, const float move_speed, const float rotate_speed)
{
    move_cam(cam, move_speed);
    rotate_cam(cam, rotate_speed);
}

#if FUNCTIONAL
static Camera2D move_cam_f(Camera2D cam, const float speed)
{
    move_cam(&cam, speed);
    return cam;
}
static Camera2D rotate_cam_f(Camera2D cam, const float speed)
{
    rotate_cam(&cam, speed);
    return cam;
}
static Camera2D update_cam_f(Camera2D cam, const float move_speed, const float rotate_speed)
{
    update_cam(&cam, move_speed, rotate_speed);
    return cam;
}
#endif // FUNCTIONAL

/* ------------------------------------------------------------------ */
/* 3D                                                                  */
/* ------------------------------------------------------------------ */
#elif GAME_D == 3

static Vec3 Cam_Info_ECS[5] = {0};

typedef struct TrackedCam
{
    Vec3 past_pos;
    Vec3 pos;
    Vec3 up;
    Vec3 right;
    Vec3 forward;
    Camera3D cam;
} TrackedCam;

static Camera3D init_cam(const Vec3 pos, const Vec3 target, const Vec3 up, const float fovy)
{
    Camera3D cam = {0};
    cam.target = target;
    cam.position = pos;
    cam.projection = CAMERA_PERSPECTIVE;
    cam.up = up;
    cam.fovy = fovy;
    return cam;
}

static Vec3 get_forward(const Camera3D cam)
{
    return Vector3Subtract(cam.target, cam.position);
}
static Vec3 get_flat(const Vec3 v)
{
    return (Vec3){.x = v.x, .y = 0, .z = v.z};
}
static Vec3 get_flat_forward(const Camera3D cam)
{
    return Vector3Normalize(get_flat(get_forward(cam)));
}
static Vec3 get_right(const Camera3D cam)
{
    return Vector3Normalize(Vector3CrossProduct(get_forward(cam), cam.up));
}

// local input: x = strafe, z = forward
static Vec3 get_move_norm(void)
{
    const signed int fwd = (int)IsKeyDown(KEY_W) - (int)IsKeyDown(KEY_S);
    const signed int strafe = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
    return Vector3Normalize((Vec3){.x = strafe, .y = 0, .z = fwd});
}
// local input -> world direction (flat forward + right)
static Vec3 get_move_world(const Camera3D cam, const Vec3 move_norm)
{
    const Vec3 fwd = Vector3Scale(get_flat_forward(cam), move_norm.z);
    const Vec3 side = Vector3Scale(get_right(cam), move_norm.x);
    return Vector3Add(fwd, side);
}
static Vec3 scale_norm(const Vec3 move_norm, const float scalar)
{
    return Vector3Scale(move_norm, scalar);
}
static Vec3 apply_move(const Vec3 move, const Vec3 pos)
{
    return Vector3Add(pos, move);
}

// call as move_pos(&pos, scale_norm(get_move_world(cam, get_move_norm()), move_speed))
// do it for cam.position AND cam.target so the view direction is kept
static void move_pos(Vec3* pos, const Vec3 scaled_move_vector)
{
    *pos = apply_move(scaled_move_vector, *pos);
}

// Q/E yaw around cam.up, speed in radians
static void rotate_cam(Camera3D* cam, const float speed)
{
    float angle = 0;
    if (IsKeyDown(KEY_Q)) angle -= speed;
    if (IsKeyDown(KEY_E)) angle += speed;
    const Vec3 fwd = Vector3RotateByAxisAngle(get_forward(*cam), cam->up, angle);
    cam->target = Vector3Add(cam->position, fwd);
}

static void update_cam(Camera3D* cam, const Vec3 target, const Vec3 pos)
{
    cam->position = pos;
    cam->target = target;
}

static TrackedCam get_TrackedCam(const Vec3 pos, const Vec3 target, const Vec3 up, const float fovy)
{
    TrackedCam cam = {0};
    cam.cam = init_cam(pos, target, up, fovy);
    cam.forward = get_forward(cam.cam);
    cam.up = up;
    cam.right = get_right(cam.cam);
    cam.pos = pos;
    cam.past_pos = pos;
    return cam;
}

static void update_TrackedCam(TrackedCam* cam, const Vec3 target, const Vec3 position_transformation)
{
    cam->past_pos = cam->pos;
    cam->pos = cam->cam.position;
    cam->forward = get_forward(cam->cam);
    cam->right = get_right(cam->cam);
    update_cam(&cam->cam, target, position_transformation);
}

// moves offset toward (or away from) the origin on each axis
static Vec3 ZOOM_IN(const Vec3 offset, const float amount)
{
    return (Vec3){
        .x = offset.x - (amount * SIGN_OF(offset.x)),
        .y = offset.y - (amount * SIGN_OF(offset.y)),
        .z = offset.z - (amount * SIGN_OF(offset.z))
    };
}
static Vec3 ZOOM_OUT(const Vec3 offset, const float amount)
{
    return ZOOM_IN(offset, -amount);
}

#endif // GAME_D