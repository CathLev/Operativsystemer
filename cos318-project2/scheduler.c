/* scheduler.c 
 * COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
 * A non-preemptive scheduler
 */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "util.h"

int scheduler_count;

// Declare queues here

void scheduler(void) {
    ++scheduler_count;
    current_running = current_running->next;
}

void do_yield(void) {
}

void do_exit(void) {
}

void block(void) {
}

void unblock(void) {
}

bool_t blocked_tasks(void) {
    return TRUE;
}
