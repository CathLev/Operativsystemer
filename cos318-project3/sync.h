/* sync.h
 * COS 318, Fall 2019: Project 3 Preemptive Scheduler
 * Definitions and types of implementation of locks, condition variables,
 * semaphores, and barriers
 */ 

#ifndef THREAD_H
#define THREAD_H

#include "queue.h"
#include "scheduler.h"

typedef struct {
    enum {
        UNLOCKED,
        LOCKED
    } status;
    node_t wait_queue;
    pcb_t* held_task;
} lock_t;

// TODO: Define the condition_t structure
typedef struct {} condition_t;

// TODO: Define the semaphore_t structure
typedef struct {} semaphore_t;

// TODO: Define the barrier_t structure
typedef struct {} barrier_t;

// Lock functions
void lock_init(lock_t *);
int lock_acquire(lock_t *);
void lock_release(lock_t *);

// Condition variable functions
void condition_init(condition_t * c);
void condition_wait(lock_t * m, condition_t * c);
void condition_signal(condition_t * c);
void condition_broadcast(condition_t * c);

// Semaphore functions
void semaphore_init(semaphore_t * s, int value);
void semaphore_up(semaphore_t * s);
void semaphore_down(semaphore_t * s);

// Barrier functions
void barrier_init(barrier_t * b, int n);
void barrier_wait(barrier_t * b);

#endif
