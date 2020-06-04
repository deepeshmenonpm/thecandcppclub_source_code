#include <iostream>
#include "heap.h"

using namespace std;

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