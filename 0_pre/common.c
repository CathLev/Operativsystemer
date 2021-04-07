

__asm__(".code16gcc");

#include "common.h"

/*
 * Returns 0 if s1 equals s2. Otherwise, returns -1. Both
 * strings must at least contain a trailing '\0'.
 */
int strcmp(char *s1, char *s2){
    
    while(*s1 == *s2){
        
        if(*s1 == '\0')
            return 0;

        s1++;
        s2++;
    }

    return -1;
}

/* 
 * Zero out size bytes starting at area.
 * */
void bzero(char *area, int size)
{
  int i;

  for (i = 0; i < size; i++)
    area[i] = 0;
}

