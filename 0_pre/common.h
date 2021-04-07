
#ifndef COMMON
#define COMMON

#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

void bzero(char *area, int size);

int strcmp(char *s1, char *s2);

#endif

