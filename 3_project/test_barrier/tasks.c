#include "scheduler.h"
#include "th.h"

struct task_info task[] = {
    // Barrier test threads
    TH(&barrier1), TH(&barrier2), TH(&barrier3),
    // This means the ready queue is never empty
    PROC(PROC1_ADDR)
};

enum {
    NUM_TASKS = sizeof task / sizeof(struct task_info)
};
