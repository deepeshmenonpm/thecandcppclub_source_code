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
	}
	
	return heap;
}

#if 0
class A
{
private:
	int a_;
public:
	explicit A(int a = 0):a_{a}
	{
		cout << "A created with : " << dec << a_ << endl;
	}
	
	~A()
	{
		cout << "A destroyed" << endl;
	}
	
	static void* operator new(size_t size)
	{
		cout << "Custom new size " << size << endl;
		Heap* heap = Heap::get_heap_instance();
		void *mem_new = heap->alloc_memory(size);
		return mem_new;
	}
		
	static void operator delete(void *mem)
	{
		cout << "Custom delete" << endl;
		Heap* heap = Heap::get_heap_instance();
		heap->dealloc_memory(mem);
		
		return;
	}
};


int main()
{
	char *mem1, *mem2, *mem3, *mem4, *mem5;
	
	Heap* heap = Heap::get_heap_instance();
	
	mem1 = reinterpret_cast<char*>(heap->alloc_memory(200));
	
	if(mem1 != nullptr)
	{
		cout << "memory allocated at " << hex << (unsigned int)mem1 << endl;
		for(int c = 0; c < 200; ++c )
		{
			mem1[c] = (char)(c);
		}
		
		mem2 = reinterpret_cast<char*>(heap->alloc_memory(100));
		
		if(mem2 != nullptr)
		{
			cout << "second memory allocated at " << hex << (unsigned int)mem2 << endl;
			for(int c = 0; c < 100; ++c )
			{
				mem2[c] = (char)(c);
			}
		}
		else
		{
			cout << "memory not allocated" << endl;
		}
		
		mem3 = reinterpret_cast<char*>(heap->alloc_memory(300));
		
		if(mem3 != nullptr)
		{
			cout << "second memory allocated at " << hex << (unsigned int)mem3 << endl;
			for(int c = 0; c < 100; ++c )
			{
				mem3[c] = (char)(c);
			}
		}
		else
		{
			cout << "memory not allocated" << endl;
		}
		
	}
	
	// heap dump 
	//Heap::heap_dump();
	
	if(true == heap->dealloc_memory(mem2))
	{
		cout << "mem2 freed" << endl;
	}
	else
	{
		cout << "mem2 not freed" << endl;
	}
	
	// allocate a new memory 
	
	mem4 = reinterpret_cast<char*>(heap->alloc_memory(30));
	
	if(mem4 != nullptr)
	{
		cout << "memory allocated at " << hex << (unsigned int)mem4 << endl;
	}
	
	mem5 = reinterpret_cast<char*>(heap->alloc_memory(30));
	
	if(mem5 != nullptr)
	{
		cout << "memory allocated at " << hex << (unsigned int)mem5 << endl;
	}
	
	{
		A *a1 = new A(5);
		delete a1;
		
		A a2(10);
	}
	
	
	cout << "main ends" << endl;
	
	return 0;	
	
}

#endif