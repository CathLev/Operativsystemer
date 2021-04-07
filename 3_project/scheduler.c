/* scheduler.c
 * COS 318, Fall 2019: Project 3 Preemptive Scheduler
 * Process scheduler for the kernel
 */

#include "common.h"
#include "interrupt.h"
#include "queue.h"
#include "printf.h"
#include "scheduler.h"
#include "util.h"
#include "syslib.h"

pcb_t *current_running;
node_t ready_queue;
node_t sleep_wait_queue;
// More variables...
volatile uint64_t time_elapsed;

// TODO: Round-robin scheduling: Save current_running before preempting
void put_current_running() {}

// Change current_running to the next task
void scheduler() {
    ASSERT(disable_count);
    while (is_empty(&ready_queue)) {     
        leave_critical();
        enter_critical();
    }
    current_running = (pcb_t *) dequeue(&ready_queue);
    ASSERT(NULL != current_running);
    ++current_running->entry_count;
}

// TODO: Blocking sleep
void do_sleep(int milliseconds) {
  ASSERT(!disable_count);
  uint64_t deadline;
  
  deadline = time_elapsed + milliseconds;
  while (time_elapsed < deadline) {}
}

// TODO: Check if we can wake up sleeping processes
void check_sleeping() {}

/* DO NOT MODIFY ANY OF THE FOLLOWING FUNCTIONS */

void do_yield() {
    enter_critical();
    put_current_running();
    scheduler_entry();
    leave_critical();
}

void do_exit() {
    enter_critical();
    current_running->status = EXITED;
    scheduler_entry();
    // No need for leave_critical() since scheduler_entry() never returns
}

void block(node_t * wait_queue) {
    ASSERT(disable_count);
    current_running->status = BLOCKED;
    enqueue(wait_queue, (node_t *) current_running);
    scheduler_entry();
}

void unblock(pcb_t * task) {
    ASSERT(disable_count);
    task->status = READY;
    enqueue(&ready_queue, (node_t *) task);
}

pid_t do_getpid() {
    pid_t pid;  
    enter_critical();
    pid = current_running->pid;
    leave_critical();
    return pid;
}

uint64_t do_gettimeofday(void) {
    return time_elapsed;
}

// Reserved for Extra Credit
priority_t do_getpriority() {
    priority_t priority;  
    enter_critical();
    priority = current_running->priority;
    leave_critical();
    return priority;
}

// Reserved for Extra Credit
void do_setpriority(priority_t priority) {
    if( priority >= 1 ) {
        enter_critical();
        current_running->priority = priority;
        leave_critical();
    }
}
