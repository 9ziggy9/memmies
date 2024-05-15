#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <sys/resource.h>

#define TITLE        "arenas"
#define SCREEN_WIDTH  960
#define SCREEN_HEIGHT 720
#define TARGET_FPS    144


#define CLOSE_WITH(CODE, MSG)                     \
  do {                                            \
    fprintf(stdout, "[INFO] %s() :: ", __func__); \
    fprintf(stdout, "%s\n", MSG);                 \
    exit(CODE);                                   \
  } while(0)                                      \

void exit_clean_procedure(int, void *);
void poll_key_presses_exit(void);

#ifdef MEM_TEST__
  typedef enum {
    EXIT_MEM_CAP_EXCEEDED = 100,
  } mem_codes;

  void exit_memcap_procedure(int code, void *args) {
    (void) args;
    switch (code) {
    case EXIT_MEM_CAP_EXCEEDED:
      fprintf(stderr, "[MEM FAIL]: lol, you suck, too much mem. ;_;\n");
      break;
    }
    exit(code);
  }
#endif

int main(void) {

#ifdef MEM_TEST__
  #define MEM_CAP_IN_BYTES 5e8 
  on_exit(exit_memcap_procedure, NULL);
  struct rlimit mem_cap = {
    .rlim_cur = MEM_CAP_IN_BYTES,
    .rlim_max = MEM_CAP_IN_BYTES,
  };
  if (setrlimit(RLIMIT_AS, &mem_cap) != 0) exit(EXIT_MEM_CAP_EXCEEDED);
#endif
  on_exit(exit_clean_procedure, NULL);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(TARGET_FPS);
  SetExitKey(KEY_NULL);

  while(!WindowShouldClose()) {
    poll_key_presses_exit();
    BeginDrawing();
      ClearBackground(DARKGRAY);
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

void poll_key_presses_exit(void) {
  if (IsKeyPressed(KEY_ESCAPE)) CLOSE_WITH(EXIT_SUCCESS, "Exit key pressed.");
  if (IsKeyPressed(KEY_Q))      CLOSE_WITH(EXIT_SUCCESS, "Exit key pressed.");
}
