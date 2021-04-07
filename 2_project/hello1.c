/* hello1.c
 * COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
 * Example calling x86 assembly from C and vice versa
 */

#include <stdio.h>

int value = 42;
int timestwo(int);

int main(int argc, char *argv[]) {
    printf("%d\n", timestwo(2));
    return 0;
}
