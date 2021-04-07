
__asm__(".code16gcc");

#include "common.h"
#include "shell.h"
#include "allocator.h"
#include "history.h"

static uint16_t cur_x, cur_y;
static uint16_t MAX_X = 0, MAX_Y = 0;

void init(void){

    char *buf, vmode;
    int read;

    /*
     * Initializing the memory allocator.
     *
     * The memory allocator is ported from the USB
     * subsystem. It will use the memory region:
     *
     * 0x8000
     * 0xFFFF
     *
     * within the current DS for memory pool.
     */
    allocator_init();

    /*
     * Initialize the history buffer with a maximum buffer
     * size (FIFO queue).
     */
    history_init(100);

    /*
     * Read the video character size parameters.
     */
    vmode = get_vmode();

    switch(vmode){
        case GMODE_EGA_VGA:
            MAX_X = 10;
            MAX_Y = 10;

            break;

        case GMODE_CGA_40_25:

            MAX_X = 40;
            MAX_Y = 25;

            break;

        case GMODE_CGA_80_25:
        case GMODE_MONO_80_25:

            MAX_X = 80;
            MAX_Y = 25;

            break;

        default:

            MAX_X = 10;
            MAX_Y = 10;

            break;
    }

    buf     =   kzalloc(MAX_X * MAX_Y + 1);
    cur_x   =   0;
    cur_y   =   0;

    clear_screen();
    set_marker(0,0);
    write_line("OS P0\n");

    do{
        
        write_line("in3000@bochs: ");

        read = read_line(buf);

        if(!strcmp(buf, "history")){
            history_write(); // 0x81b9
        }
        if(!strcmp(buf, "clear")){ //0x81e0
            clear_screen();
        }
    } while(1);
    
}

int read_line(char *buf){
    
    int pos = 0;
    char last;

    do{
        set_marker(cur_x, cur_y);

        last        =   get_char(); // pb 0x824a i think

        if(last != '\r'){
            write_char(last);
            buf[pos++]    =   last;
        }

        history_put(last);

        if(cur_x + 1 == MAX_X){
            cur_y = (cur_y + 1) % MAX_Y;
        }

        cur_x = (cur_x + 1) % MAX_X;

    }while((last != '\r') && (pos < MAX_X * MAX_Y));

    cur_x = 0;
    cur_y = (cur_y + 1) % MAX_Y;
    set_marker(cur_x, cur_y);

    buf[pos] = '\0';

    return pos;

}

void clear_screen(void){
    
    uint16_t i, j;

    for(i = 0; i < MAX_X; i++){
        for(j = 0; j < MAX_Y; j++){
            
            set_marker(i, j);
            write_char('\0');
        }
    }

    cur_x = 0;
    cur_y = 0;
    set_marker(cur_x, cur_y);
}

void write_line(char *buf){

    while(*buf != '\0'){
        
        switch(*buf){
            
            case '\n':

                cur_y = (cur_y + 1) % MAX_Y;
                cur_x = 0;


                break;

            case '\r':

                cur_y = (cur_y + 1) % MAX_Y;
                cur_x = 0;


                break;

            default:

                write_char(*buf);
                
                if(cur_x + 1 == MAX_X){
                    cur_y = (cur_y + 1) % MAX_Y;
                }

                cur_x = (cur_x + 1) % MAX_X;

                break;
        }

        buf++;
        set_marker(cur_x, cur_y);
    }



    
}








