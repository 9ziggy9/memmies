#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>

#include "config.h"

#define MEM_MAX_CAP (1024 * 1024 * 500)
#define LOAD_PROC_INFO
#include "proc.h"

#define CLOSE_WITH(CODE, MSG)                     \
  do {                                            \
    fprintf(stdout, "[INFO] %s() :: ", __func__); \
    fprintf(stdout, "%s\n", MSG);                 \
    exit(CODE);                                   \
  } while(0)                                      \

void exit_clean_procedure(int, void *);
void poll_key_presses_exit(Camera3D *, Vector3 *);
Texture2D gen_mdl_checkerboard(void);

#define SZ_WORLD    100
#define SZ_CHECKER  4

/* #define WORLD_ORIGIN ((Vector3){SZ_WORLD * 0.5f, 0, SZ_WORLD * 0.5f}) */

#define WORLD_ORIGIN ((Vector3){0, 0, 0})
#define CAM_ORIGIN \
  ((Vector3){SZ_WORLD * 0.5f, SZ_WORLD * 0.40f, SZ_WORLD * 0.75f})

Camera3D cam_init_scene(void) {
  return (Camera3D) {
    .position   = CAM_ORIGIN,
    .target     = WORLD_ORIGIN,
    .up         = (Vector3){0.0f, 1.0f, 0.0f},
    .fovy       = 45.0f,
    .projection = CAMERA_ORTHOGRAPHIC,
  };
}

Texture2D txtr_gen_checkerboard(void) {
  RenderTexture2D target = LoadRenderTexture(SZ_WORLD * SZ_CHECKER,
                                             SZ_WORLD * SZ_CHECKER);
  BeginTextureMode(target);
    for (int z = 0; z < SZ_WORLD; z++) {
      for (int x = 0; x < SZ_WORLD; x++) {
        DrawRectangle(x * SZ_CHECKER, z * SZ_CHECKER,
                          SZ_CHECKER, SZ_CHECKER,
                      ((x+z) % 2) ? RAYWHITE : DARKGRAY);
      }
    }
  EndTextureMode();
  return target.texture;
}

int main(void) {
  on_exit(exit_clean_procedure, NULL);
  PROC_INFO_BOOTSTRAP();
  PROC_INFO_CAP_MEM(MEM_MAX_CAP);

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(TARGET_FPS);
  SetExitKey(KEY_NULL);

  Texture2D txtr_cb = txtr_gen_checkerboard();
  Model mdl_cb = LoadModelFromMesh(GenMeshPlane(SZ_WORLD * 1.5f,
                                                SZ_WORLD * 1.5f,
                                                1, 1));
  mdl_cb.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = txtr_cb;

  Camera3D cam_scene = cam_init_scene();

  Vector3 my_position = {0, 1.0f, 0};
  Vector3 world_position = WORLD_ORIGIN;

  while(!WindowShouldClose()) {
    poll_key_presses_exit(&cam_scene, &world_position);
    ClearBackground(GREEN);
    BeginDrawing();
      BeginMode3D(cam_scene);
          DrawModel(mdl_cb, world_position, 1.0f, WHITE);
          DrawSphere(my_position, 1.0f, BLUE);
        EndMode3D();
      PROC_INFO_DRAW(PROC_INFO_FLAG_ALL);
    EndDrawing();
  }
  CloseWindow();
  return EXIT_SUCCESS;
}

void exit_clean_procedure(int code, void *resources) {
  (void) resources;
  CloseWindow();
  switch (code) {
  case EXIT_SUCCESS: fprintf(stdout, "[EXIT] clean exit. :D\n"); break;
  case EXIT_FAILURE: fprintf(stderr, "[EXIT] bad exit! >.<\n");  break;
  }
  exit(code);
}

void poll_key_presses_exit(Camera3D *cam, Vector3 *pos) {
  if (IsKeyDown(KEY_W)) pos->z += 0.1f;
  if (IsKeyDown(KEY_A)) pos->x += 0.1f;
  if (IsKeyDown(KEY_S)) pos->z -= 0.1f;
  if (IsKeyDown(KEY_D)) pos->x -= 0.1f;
  if (IsKeyDown(KEY_UP))   cam->position.y -= 0.5f;
  if (IsKeyDown(KEY_DOWN)) cam->position.y += 0.5f;
  if (IsKeyPressed(KEY_ESCAPE)) CLOSE_WITH(EXIT_SUCCESS, "Exit key pressed.");
  if (IsKeyPressed(KEY_Q))      CLOSE_WITH(EXIT_SUCCESS, "Exit key pressed.");
}
