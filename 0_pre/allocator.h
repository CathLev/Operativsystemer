#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#define KERNEL_ALLOC_START 0x8000     /* Mem page top value    */
#define KERNEL_ALLOC_STOP  0xFFFF
  
void *kzalloc(int size);
void *kzalloc_align(int size, int alignment);
void kfree(void *elem);

void allocator_init();

#endif
