/* common.h
 * COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
 * Common definitions and types
 */

#ifndef COMMON_H
#define COMMON_H

#ifndef NULL
#define NULL ((void*) 0)
#endif

// Physical address of the text mode display 
#define SCREEN_ADDR ((short *) 0xb8000)

// Used in threads and processes as argument for delay
#define DELAY_VAL 2

// If the expression p fails, print the source file and line number along with
// the text s and then hang the OS; processes should use this macro
#define ASSERT2(p, s) \
    do { \
        if (!(p)) { \
            print_str(0, 0, "Assertion failure: "); \
            print_str(0, 19, s); \
            print_str(1, 0, "file: "); \
            print_str(1, 6, __FILE__); \
            print_str(2, 0, "line: "); \
            print_int(2, 6, __LINE__); \
            asm volatile ("cli"); \
            while (1); \
        } \
    } while(0)

// The #p tells the compiler to copy the expression p into a text string and
// use this as an argument; if the expression fails, print the expression as a
// message
#define ASSERT(p) ASSERT2(p, #p)

// Gives us the source file and line number where we decide to hang the OS
#define HALT(s) ASSERT2(FALSE, s)

// Typedefs of commonly used types
typedef enum {
    FALSE, TRUE
} bool_t;

typedef signed char int8_t;
typedef short int int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

// Processor speed
enum {
    MHZ = 2394,
};

#endif
