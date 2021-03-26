#include "scheduler.h"
#include "th.h"

struct task_info task[] = {
    // Clock and status thread
    TH(&clock_thread),
    TH(&ps_thread),
    PROC(PROC1_ADDR),
    PROC(PROC2_ADDR),
};

enum {
    NUM_TASKS = sizeof task / sizeof(struct task_info)
};
