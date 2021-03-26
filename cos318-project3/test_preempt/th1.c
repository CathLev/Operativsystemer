#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "th.h"
#include "util.h"
#include "printf.h"

// This thread runs indefinitely, which means that the scheduler should never
// run out of processes
void clock_thread(void) {
    while (TRUE) {
        printf(24, 50, "Time (in seconds) : %d",
               (int) do_gettimeofday() / 1000);
        do_yield();
    }
}
