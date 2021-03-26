#include "common.h"
#include "scheduler.h"
#include "sync.h"
#include "th.h"
#include "util.h"
#include "printf.h"

/* BASIC PRODUCER-CONSUMER TEST */

// Thread status
#define RUNNING	0
#define OK		1
#define FAILED	2

static void print_trace(int thread, int status);

static int shared_var = 0;
static int exit_count = 0;     // Number of threads that have exited
static int init = FALSE;       // Is lock l initialized
static lock_t l;               // Lock for the monitor
static condition_t c0mod3;     // thread3 waits for this one
static condition_t cnot0mod3;  // thread2 waits for this one

// thread 2 waits until shared_var % 3 != 0 (cnot0mod3)
void thread2(void) {
    int tmp, i;

    // This thread has to initialize the locks and condition variables for the
    // monitor
    lock_init(&l);
    condition_init(&c0mod3);
    condition_init(&cnot0mod3);

    // The other thread waits for init to become TRUE, which signals that the
    // locks and condition variables are initialized
    init = TRUE;
    
    for (i = 0; i < 200; i++) {
        ASSERT(0 == lock_acquire(&l));

        // Wait until thread3 signals that shared_var % 3 != 0
        // Remember that condition_wait unlocks l which allows thread2 to enter
        // the monitor
        while (shared_var % 3 == 0) {
            condition_wait(&l, &cnot0mod3);
        }
        ASSERT(shared_var % 3 != 0);
        
        // After being signaled, thread2 acquires the lock so it is safe to use
        // shared_var
        tmp = shared_var;
        if (i % 13 == 0) {
            // Yielding inside the monitor might trigger some errors in bad
            // implementations of the locks and condition variables
            do_yield();
        }

        // shared_var should be equal to tmp
        // If the synchronization primitives are not working correctly then
        // shared_var > tmp, because thread3 has increased shared_var
        shared_var = tmp + 1;

        if (shared_var % 3 == 0) {
            // Signal or broadcast condition for thread3
            if (shared_var < 20)
                condition_signal(&c0mod3);
            else
                condition_broadcast(&c0mod3);
        }
        print_trace(2, RUNNING);
        
        lock_release(&l);
    }
    
    ASSERT(0 == lock_acquire(&l));
    exit_count++;
    
    // When both threads have finished we print the result
    if (exit_count == 2) {
        print_trace(2, (shared_var == 300) ? OK : FAILED);
    }
    
    lock_release(&l);
    do_exit();
}

// thread 3 waits for condition shared_var % 3 != 0 (c0mod3) 
void thread3(void) {
    int tmp, i;

    // Wait for thread2 to finish initializing the monitor
    while (!init) {
        do_yield();
    }
    
    for (i = 0; i < 100; i++) {
        ASSERT(0 == lock_acquire(&l));

        // Wait until thread2 signals that shared_var % 3 == 0
        while (shared_var % 3 != 0) {
            condition_wait(&l, &c0mod3);
        }
        ASSERT(shared_var % 3 == 0);
        
        // We still have the lock so it is safe to use shared_var
        tmp = shared_var;
        if (i % 17 == 0) {
            // Yielding inside the monitor might trigger some errors in bad
            // implementations of the locks and condition variables
            do_yield();
        }
        
        // shared_var should be equal to tmp
        shared_var = tmp + 1;

        if (shared_var % 3 != 0) {
            // Every now and then, send a signal or broadcast to the other thread
            if (shared_var < 20)
                condition_signal(&cnot0mod3);
            else
                condition_broadcast(&cnot0mod3);
        }
        print_trace(3, RUNNING);
        
        lock_release(&l);
    }
    
    ASSERT( 0 == lock_acquire(&l) );
    exit_count++;
    
    // When both threads have finished print the result
    if (exit_count == 2) {
        print_trace(3, (shared_var == 300) ? OK : FAILED);
    }
    
    lock_release(&l);
    do_exit();
}

#define LINE 13
#define COL 48

static void print_trace(int thread, int status) {
    int line;

    line = LINE + thread - 2;
    if (status == RUNNING) {
        printf(line, COL, "L&CV thread %d: %d", do_getpid(), shared_var);
    } else {
        printf(line, COL, "L&CV thread %d: %s", do_getpid(),
               OK == status ? "Both passed" : "Failed");
    }
    do_sleep(100);
}
