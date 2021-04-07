/* th1.c
 * COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
 * Simple counter program to check the functionality of do_yield()
 */

#include "scheduler.h"
#include "th.h"
#include "util.h"

static void print_counter(void);

// This thread runs indefinitely
void clock_thread(void) {
    unsigned int time;
    unsigned long long int ticks;
    unsigned int start_time;

    // To show time since last boot, remove all references to start_time
    ticks = get_timer() >> 20;         // Divide by 2^20 = 10^6 (1048576)
    start_time = ((int) ticks) / MHZ;  // Divide by CPU clock frequency in MHz
 
    while (1) {
        ticks = get_timer() >> 20;   // Divide by 2^20 = 10^6 (1048576)
        time = ((int) ticks) / MHZ;  // Divide by CPU clock frequency in MHz

        print_str(24, 50, "Time (in seconds) : ");
        print_int(24, 70, time - start_time);
        print_counter();
        do_yield();
    }
}

static void print_counter(void) {
    static int counter = 0;

    print_str(20, 0, "Thread  1 (time)      : ");
    print_int(20, 25, counter++);
}
