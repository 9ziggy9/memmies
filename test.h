#ifdef MEM_TEST__
  #include <sys/resource.h>
  #include <raylib.h>
  #include <string.h>

  #include "config.h"

  #define DEBUG_FONT_SZ 16

  typedef enum {
    EXIT_MEM_CAP_EXCEEDED = 100,
  } mem_codes;

  static struct rusage mem_used;

  void debug_info(struct rusage *m, Color clr) {
    char txt_mem[32];
    char txt_fps[32];
    snprintf(txt_mem, sizeof(txt_mem), "[ mem: %.2lf MB ]",
            (getrusage(RUSAGE_SELF, m) == 0)
            ? (double)m->ru_maxrss * (1.0e-3)
            : -1.0);
    snprintf(txt_fps, sizeof(txt_fps), "[ fps: %d ]", GetFPS());
    int txt_mem_width = MeasureText(txt_mem, DEBUG_FONT_SZ) + 4;
    int txt_fps_width = MeasureText(txt_fps, DEBUG_FONT_SZ) + 4;
    DrawText(txt_fps, SCREEN_WIDTH - txt_fps_width, 1, DEBUG_FONT_SZ, clr);
    DrawText(txt_mem, SCREEN_WIDTH - txt_mem_width - txt_fps_width,
             1, DEBUG_FONT_SZ, clr);
  }

  void exit_memcap_procedure(int code, void *args) {
    (void) args;
    switch (code) {
    case EXIT_MEM_CAP_EXCEEDED:
      fprintf(stderr, "[MEM FAIL]: lol, you suck, too much mem. ;_;\n");
      break;
    }
    exit(code);
  }

  #define MEM_TEST_BOOTSTRAP() do {                                         \
      on_exit(exit_memcap_procedure, NULL);                                 \
      struct rlimit mem_cap =                                               \
          {.rlim_cur = MEM_CAP_IN_BYTES, .rlim_max = MEM_CAP_IN_BYTES};     \
      if (setrlimit(RLIMIT_AS, &mem_cap) != 0) exit(EXIT_MEM_CAP_EXCEEDED); \
    } while(0)

  #define MEM_TEST_INFO_BAR() do {                     \
      DrawRectangle(0, 0, SCREEN_WIDTH, 16, DARKBLUE); \
      debug_info(&mem_used, RAYWHITE);                 \
    } while(0)

#else
  #define MEM_TEST_BOOTSTRAP() do {} while(0)
  #define MEM_TEST_INFO_BAR()  do {} while(0)
#endif // MEM_TEST__
