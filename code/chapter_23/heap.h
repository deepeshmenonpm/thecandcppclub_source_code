#ifndef HEAP_H
#define HEAP_H

class Heap
{
private:
	struct mem_info_t
	{
		struct mem_info_t *next;
		size_t size;
		bool is_free;
	};
	
	mem_info_t *root;
	char *heap_end;
private:
	Heap();
	static void* operator new(size_t size);
public:
	~Heap();
	void clear_heap_memory();	
	void* alloc_memory(size_t size);
	bool dealloc_memory(void *this_mem);
	void heap_dump();
	static Heap* get_heap_instance(void);
	
};




#endif /*HEAP_H*/
