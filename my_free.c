// ADD YOUR IMPLEMENTATION IN THIS FILE
#include "my_mem_allocator.h"

extern ALLOC_POLICY alloc_policy; // default: first-fit
extern void * my_heap; // start address of the heap space that this allocator is managing
extern BLOCK_HDR * fl_head; // head pointer to the free list

void my_free(void *ptr)
{   		
	//double free
	//if ptr minus the size of a void ptr doesn't equal MAGIC_NUM, then the block is already been freed
	if(*((int *)((unsigned long)ptr - sizeof(void *))) != MAGIC_NUM){
		printf("double free\n");
		return;
	}
	int size = (*((int *)((unsigned long)ptr - sizeof(BLOCK_HDR))));
	BLOCK_HDR *fb = (void *)((unsigned long)ptr - sizeof(BLOCK_HDR));
	fb->size = size;
	BLOCK_HDR *current = fl_head;
	BLOCK_HDR *low = fl_head;

	//if the block's address being added to the free list < head address, use this
	if((void *)current > (void *)fb){
		fb->next = fl_head;
		fl_head = fb;
	}
	//else go through the rest of the free list
	else{
		//continue looping until the current block address is not lower than the free block address
		while(current->next){
			if((void *)current < (void *)fb){
				low = current;
			}
			current = current->next;
		}
		fb->next = low->next;
		low->next = fb;
	}

	//free block is added, so coalecging is performed
	current = fl_head;
	BLOCK_HDR *tnext;
	while(current->next){
		tnext = current->next;
		if((void *)current + current->size + sizeof(BLOCK_HDR) == (void *)tnext){
			//coalesce current and next block if current+size == next free block address
			current->size += tnext->size + sizeof(BLOCK_HDR);
			current->next = tnext->next;
			current = fl_head; //go again
		}
		else{
			current = current->next; //continue
		}
	}

	return;
}