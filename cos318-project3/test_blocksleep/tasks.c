#include "scheduler.h"
#include "th.h"

struct task_info task[] = {
    TH(&thread_1),
    TH(&thread_2)
};

enum {
    NUM_TASKS = sizeof task / sizeof(struct task_info)
};
