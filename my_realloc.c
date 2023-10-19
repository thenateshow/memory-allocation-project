// ADD YOUR IMPLEMENTATION IN THIS FILE
#include "my_mem_allocator.h"

extern ALLOC_POLICY alloc_policy; // default: first-fit
extern void * my_heap; // start address of the heap space that this allocator is managing
extern BLOCK_HDR * fl_head; // head pointer to the free list

void *my_realloc(void *ptr, size_t size)
{
	if (*((int *)((unsigned long)ptr - sizeof(void *))) != MAGIC_NUM){
		printf("old pointer invalid\n");
		return NULL;
	}
	int ptr_size = (*((int *)((unsigned long)ptr - sizeof(BLOCK_HDR))));
	//new size is smaller than original size
	if(size < ptr_size){
		if(ptr_size > size + sizeof(BLOCK_HDR)){
			BLOCK_HDR * free_block = (BLOCK_HDR *)((unsigned long)ptr + size);
			free_block->size = ptr_size - size - sizeof(BLOCK_HDR);
			free_block->next = (void *)MAGIC_NUM;
			*((int *)(ptr - (sizeof(BLOCK_HDR)))) = size;
			my_free((void *)(((unsigned long)free_block) + sizeof(BLOCK_HDR)));
		}
		return ptr;
	}
	//new size larger than original size
	else{
		void *block = my_malloc(size);
		memcpy(block, ptr, ptr_size);
		my_free(ptr);
		return block;
	}
}