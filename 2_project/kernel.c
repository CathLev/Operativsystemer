/* kernel.c
 * COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
 * Entry point that runs when the bootloader transfers control to the kernel
 */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "th.h"
#include "util.h"

#include "tasks.c"

#define NUM_REGISTERS 8

pcb_t *current_running;

static pcb_t pcbs[NUM_TASKS];

// Kernel entry point must be the first function in the file
void _start(void) {
    // Set up the single entry point for system calls
    *ENTRY_POINT = &kernel_entry;

    clear_screen(0, 0, 80, 25);

    // Initialize the pcbs and the ready queue
    uint16_t i;
    ASSERT(NUM_TASKS > 0);
    for (i = 0; i < NUM_TASKS; i++) {
      pcbs[i].esp = STACK_MIN + (STACK_SIZE*(i+1));
      ASSERT(pcbs[i].esp <= STACK_MAX);
      // Push entry point onto the stack
      push_to_stack(&pcbs[i].esp, task[i]->entry_point);
      // Push zeroes onto the stack, so registers are initialized at 0
      uint16_t j;
      for (j = 0; j < NUM_REGISTERS; j++)
	push_to_stack(&pcbs[i].esp, 0);
      pcbs[i].pid = i;
      pcbs[i].t = 0;
      if (i == NUM_TASKS - 1) {
	pcbs[i].next = &pcbs[0];
      } else {
	pcbs[i].next = &pcbs[i+1];
      }
    }
    // Dummy pcb to point to the first task for loading
    pcb_t dummy;
    dummy.next = &pcbs[0];
    current_running = &dummy;

    // Schedule the first task
    scheduler_count = 0;
    scheduler_entry();

    // We should never reach here
    ASSERT(0);
}

void push_to_stack(uint32_t *esp, uint32_t val) {
  *esp -= sizeof(val);
  *(uint32_t *)*esp = val;
}
