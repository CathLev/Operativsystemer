
#ifndef HISTORY
#define HISTORY

#include "common.h"
#include "shell.h"

typedef
struct character{
    char s;         // The actual char
    
    struct character *next;
    struct character *prev;
} char_t;

void history_init(int size);

void history_put(char s);

/*
 * Writes history buffer to screen
 */
void history_write(void);

#endif
