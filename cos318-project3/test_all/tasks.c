#include "scheduler.h"
#include "th.h"

struct task_info task[] = {
  // Clock and status thread
  TH(&clock_thread),
  // Lock and condition variable test threads
  TH(&thread2), TH(&thread3),
  // Dining philosopher threads
  TH(&num), TH(&caps), TH(&scroll_th),
  // Barrier test threads
  TH(&barrier1), TH(&barrier2), TH(&barrier3),
  // Plane process
  PROC(PROC1_ADDR),
  // Calculation process
  PROC(PROC2_ADDR)
};

enum {
    NUM_TASKS = sizeof task / sizeof(struct task_info)
};
