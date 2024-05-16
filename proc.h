#ifndef PROC_INFO_H_
#define PROC_INFO_H_

#include <sys/resource.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "config.h"

typedef enum {
  EXIT_PROC_INFO_READ_FAIL = 200,
  EXIT_PROC_MEM_CORRUPTED,
} proc_info_cd;

struct proc_info_t {
  pid_t    pid;
  uintmax_t virt;  // total virtual memory, swap, shared, etc
  uintmax_t ram;   // mem in ram, including shared libs
  uintmax_t heap;  // heap allocs, malloc, calloc, etc
  uintmax_t stack; // total stack memory
  uintmax_t text;  // code/text segment information
};

void PROC_INFO_BOOTSTRAP(void);
void PROC_INFO_CAP_MEM(size_t);

#ifdef LOAD_PROC_INFO
#define DEBUG_FONT_SZ 16

static struct rlimit MEM_CAP;
static FILE *PROC_FILE;

struct proc_info_t PROC_INFO;

static void open_proc_file(void) {
  char path[256];
  PROC_INFO.pid = getpid();
  snprintf(path, sizeof(path), "/proc/%d/status", PROC_INFO.pid);
  PROC_FILE = fopen(path, "r");
  if (PROC_FILE == NULL) exit(EXIT_PROC_INFO_READ_FAIL);
}

static void populate_proc_info(void) {
  #define MAX_LINE_LEN 256
  char line[MAX_LINE_LEN];
  while (fgets(line, sizeof(line), PROC_FILE)) {
    if (strncmp(line, "VmSize:", 7) == 0)
      sscanf(line + 7, "%lu kB", &PROC_INFO.virt);
    else if (strncmp(line, "VmRSS:", 6) == 0)
      sscanf(line + 6, "%lu kB", &PROC_INFO.ram);
    else if (strncmp(line, "VmData:", 7) == 0)
      sscanf(line + 7, "%lu kB", &PROC_INFO.heap);
    else if (strncmp(line, "VmStk:", 6) == 0)
      sscanf(line + 6, "%lu kB", &PROC_INFO.stack);
    else if (strncmp(line, "VmExe:", 6) == 0)
      sscanf(line + 6, "%lu kB", &PROC_INFO.text);
  }
}

static void exit_proc_info(int code, void *args) {
  (void) args;
  fclose(PROC_FILE);
  if (code >= EXIT_PROC_INFO_READ_FAIL && code < 300) {
    switch (code) {
    case EXIT_PROC_INFO_READ_FAIL:
      fprintf(stderr, "[ERROR]: could not read from proc file.\n");
      break;
    case EXIT_PROC_MEM_CORRUPTED:
      fprintf(stderr, "[ERROR]: rlimit syscall lead to corrupt mem.\n");
      break;
    default:
      fprintf(stderr, "[ERROR]: could not read from proc file.\n");
      break;
    }
  }
  fprintf(stdout, "[INFO]: proc.h resources deallocated ...\n");
}

void PROC_INFO_BOOTSTRAP(void) {
  on_exit(exit_proc_info, NULL);
  open_proc_file();
  populate_proc_info();
}

void PROC_INFO_CAP_MEM(size_t num_bytes) {
  MEM_CAP.rlim_cur = num_bytes;
  MEM_CAP.rlim_max = num_bytes;
  if (setrlimit(RLIMIT_AS, &MEM_CAP) != 0) exit(EXIT_PROC_MEM_CORRUPTED);
}

/* void debug_info(Color clr) { */
/*   char txt_mem[512] = "MEM: "; */
/*   char txt_fps[32]; */
/*   proc_info_get(txt_mem); */
/*   snprintf(txt_fps, sizeof(txt_fps), "[ fps: %d ]", GetFPS()); */
/*   int txt_mem_width = MeasureText(txt_mem, DEBUG_FONT_SZ) + 4; */
/*   int txt_fps_width = MeasureText(txt_fps, DEBUG_FONT_SZ) + 4; */
/*   DrawText(txt_fps, SCREEN_WIDTH - txt_fps_width, 1, DEBUG_FONT_SZ, clr); */
/*   DrawText(txt_mem, SCREEN_WIDTH - txt_mem_width - txt_fps_width, */
/*             1, DEBUG_FONT_SZ, clr); */
/* } */

/* #define MEM_TEST_INFO_BAR() do {                     \ */
/*     DrawRectangle(0, 0, SCREEN_WIDTH, DEBUG_FONT_SZ, \ */
/*                   GREEN);                            \ */
/*     debug_info(RAYWHITE);                            \ */
/*   } while(0) */

#endif // LOAD_PROC_INFO
#endif // PROC_INFO_H_
