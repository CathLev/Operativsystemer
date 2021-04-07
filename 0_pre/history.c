
__asm__(".code16gcc");

#include "history.h"
#include "allocator.h"

static unsigned int max_size, cur_size;
static char_t *ll_start;
struct character *ll_current;

void history_init(int size){

    max_size = size;
    cur_size = 0; // Have not used cur_size yet
    ll_start = NULL;
    ll_current = NULL;
    
}

/*
 * Puts a character into the history buffer. The buffer
 * should be implemented as a FIFO queue.
 *
 * There is only a single consumer and producer, and hence
 * no race conditions, allowing for a very simple
 * implementation without locks.
 */
void history_put(char input_s){

    if (ll_start == NULL)
    {
        ll_start = (struct character*) kzalloc(sizeof(struct character));
        ll_start->s = input_s;
        ll_current= ll_start;
    }

    else
    {
    
    struct character *ll_next;
    ll_next = (struct character*) kzalloc(sizeof(struct character));
    ll_next->s = input_s;
    ll_current->next = ll_next;
    ll_next->prev = ll_current;
    ll_current= ll_next;
    }

    cur_size++;

}

void history_write(void){ // 0x8703
    struct character *ll_element = (struct character*) kzalloc(sizeof(struct character));
    ll_element = ll_start;

    for (int i = 0; i < cur_size -1; i++)
    {
        write_line(&ll_element->s);

        ll_element = ll_element->next;
    }
    write_line(&ll_element->s); // last element doesn't have a next
    
}



