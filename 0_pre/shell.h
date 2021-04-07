
#ifndef DISPLAY
#define DISPLAY

#include "common.h"

#define GMODE_EGA_VGA 0x00
#define GMODE_CGA_40_25 0x01
#define GMODE_CGA_80_25 0x02
#define GMODE_MONO_80_25 0x03

char get_vmode(void) __attribute__((cdecl));

/*
 * Assembly call to get a character. The function does this
 * by triggering BIOS interrupts. 
 *
 * Blocking (i.e. doesn't return until a key input is
 * received).
 */
char get_char(void) __attribute__((cdecl));

/*
 * Assembly call to write a character to a specific position
 * in the shell (set by set_marker()).
 */
void write_char(char s) __attribute__((cdecl));

/*
 * Sets the cursor marker at the specified position
 */
void set_marker(uint16_t x, uint16_t y) __attribute((cdecl));

/*
 * Reads a line from input, terminated by a carriage return.
 * Caller must ensure there is enough space in the buffer.
 *
 * Also logs all character input to the ring buffer. 
 */
int read_line(char *buf);

/*
 * Writes a line to output, terminated by a carriage return.
 * 
 * Also logs all character output to the ring buffer. 
 */
void write_line(char *buf);

void clear_screen(void);

#endif
