#ifndef TH_H
#define TH_H

void clock_thread(void);

// Threads to test condition variables and locks
void thread2(void);
void thread3(void);

// Threads to test semaphores
void num(void);
void scroll_th(void);
void caps(void);

// Threads to test barriers
void barrier1(void);
void barrier2(void);
void barrier3(void);

#endif
