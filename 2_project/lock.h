/* lock.h
 * COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
 * Definitions and types for basic mutual exclusion
 */

#ifndef LOCK_H
#define LOCK_H

typedef struct {
    enum {
        UNLOCKED,
        LOCKED,
    } status;
} lock_t;

void lock_init(lock_t *);
void lock_acquire(lock_t *);
void lock_release(lock_t *);

#endif
