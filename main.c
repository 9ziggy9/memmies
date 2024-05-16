#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>

#include "config.h"

#define LOAD_PROC_INFO
#include "proc.h"

#define CLOSE_WITH(CODE, MSG)                     \
  do {                                            \
    fprintf(stdout, "[INFO] %s() :: ", __func__); \
    fprintf(stdout, "%s\n", MSG);                 \
    exit(CODE);                                   \
  } while(0)                                      \

void exit_clean_procedure(int, void *);
void poll_key_presses_exit(void);

int main(void) {
  on_exit(exit_clean_procedure, NULL);

#define MEM_CAP_500_MB (1024 * 1024 * 500)
  PROC_INFO_BOOTSTRAP();
  PROC_INFO_CAP_MEM(MEM_CAP_500_MB);

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(TARGET_FPS);
  SetExitKey(KEY_NULL);

  while(!WindowShouldClose()) {
    poll_key_presses_exit();
    BeginDrawing();
      ClearBackground(DARKGRAY);
    EndDrawing();
    void *prepare_to_die = malloc(50000);
    if (prepare_to_die == NULL) {
      fprintf(stderr, "memory shit itself in fear. O.O\n");
    }
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

void poll_key_presses_exit(void) {
  if (IsKeyPressed(KEY_ESCAPE)) CLOSE_WITH(EXIT_SUCCESS, "Exit key pressed.");
  if (IsKeyPressed(KEY_Q))      CLOSE_WITH(EXIT_SUCCESS, "Exit key pressed.");
}
