#include <iostream>
#include <iomanip>
#include "heap.h"

using namespace std;

static const size_t max_heap_size = (10 * 1024); // 10 KB

static char heap_memory[max_heap_size];

static char heap_object[sizeof(Heap)];


Heap::Heap()
{
	clear_heap_memory();
	
	// set up the root node mem info structure 
	root = reinterpret_cast<mem_info_t*>(&heap_memory[0]);
	root->next = nullptr;
	root->size = max_heap_size;
	root->is_free = true;	
	
	heap_end = &heap_memory[max_heap_size-1];
	
	cout << "heap initialized" << endl;
}

Heap::~Heap()
{
	clear_heap_memory();
	cout << "Heap destroyed" << endl;
}

void Heap::clear_heap_memory()
{
	for (auto &byte : heap_memory)
	{
		byte = static_cast<char>(0);
	}
	
	return;
}

void* Heap::alloc_memory(size_t size)
{
	char *mem = nullptr;
	mem_info_t *block_info = root;
	// first check whether any free blocks are there 
	do
	{
		// check for free blocks and see whether the size is adequate 
		if( (block_info->is_free == true) && (block_info->size >= size) )
		{
			// found a node 
			// 1. allocate memory block 
			mem = reinterpret_cast<char*>(block_info + 1);
			// 2. update this block information 
			block_info->size = size;
			block_info->is_free = false;
			
			// 3. update mem_info for next block 
			if(block_info->next != nullptr)
			{
				mem_info_t *temp_next = reinterpret_cast<mem_info_t*>(block_info->next);
				// check of enough space is there to accommodate a new empty block in between
				size_t rem_size = size_t( ((size_t)temp_next) - ((size_t)(mem + size)) + ((size_t)1) );
				
				if (rem_size > sizeof(mem_info_t))
				{
					mem_info_t *temp_new = reinterpret_cast<mem_info_t*>(mem + size);
					temp_new->next = temp_next;
					temp_new->is_free = true;
					temp_new->size = rem_size - sizeof(mem_info_t);
					block_info->next = temp_new;
				}
				// else no block can be allocated here 					
			}
			// there is no more block allocated here 
			// check if a new block can be allocated here 
			else if( size_t(mem) + size < size_t(heap_end) )
			{
				
				size_t rem_size = size_t(heap_end) - ( size_t(mem) + size) + size_t(1);
				
				if (rem_size > sizeof(mem_info_t))
				{
					mem_info_t *temp_new = reinterpret_cast<mem_info_t*>(mem + size);
					temp_new->next = nullptr;
					temp_new->is_free = true;
					temp_new->size = rem_size - sizeof(mem_info_t);
					block_info->next = temp_new;
				}
				
			}
			
			break; // exit while loop 
		}
		
		block_info = block_info->next;
		
	}while(block_info != nullptr);
	
	return reinterpret_cast<void*>(mem);
}

bool Heap::dealloc_memory(void *this_mem)
{
	char *mem = reinterpret_cast<char*>(this_mem);
	bool status = false;
	if( (unsigned int)(mem) < (unsigned int)(heap_memory))
	{
		cout << "Invalid memory" << endl;
		return status;
	}
	
	mem_info_t *block_info = root;
	mem_info_t *required_mem_block = reinterpret_cast<mem_info_t*>(mem) - 1;
	
	do
	{
		if(block_info == required_mem_block)
		{
			cout << "found the memory to be deallocated at " << hex << (unsigned int)required_mem_block << endl;
			block_info->is_free = true;
			// clear the memory
			for(int c = 0; c < block_info->size; ++c)
			{
				mem[c] = char(0);
			}
			status = true;
			break; //exit while loop 
		}
		
		block_info = block_info->next;			
		
	}while(block_info != nullptr);
	
	// TODO : To merge with consecutive free blocks 
	
}

void Heap::heap_dump()
{
	int i;
	cout << "\nHeap dump" << endl;
	
	for(auto byte: heap_memory)
	{
		cout << "Byte " << dec << i << " : " << hex << "0x" << (0xFF & (unsigned int)byte) << endl;
	}
	
	cout << endl;
}


static void* Heap::operator new(size_t size)
{
	// size is unused 
	return reinterpret_cast<void*>(heap_object);
}

Heap* Heap::get_heap_instance()
{
	static bool is_init = false;
	static Heap *heap = nullptr;
	
	if(is_init == false)
	{
		heap = new Heap();
		is_init = true;
	}
	
	return heap;
}
