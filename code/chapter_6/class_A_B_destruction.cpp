#include <iostream>

using namespace std;

class A
{
private:
	int *memory;
public:
	explicit A(int size) : memory{new int[size]}
	{
		cout << "A is created!" << endl;
	}
	
	virtual ~A()
	{
		cout << "A is destroyed" << endl;
		
		if(memory != nullptr)
		{
			delete []memory;
		}
		
	}
	
	virtual void do_something() 
	{
		// do something 
		cout << "Operation not supported or something went wrong!" << endl;
		throw 1; 
	}
	
};

class B: public A
{
private:
	int *memory;
public:
	B(int size): A(size), memory{new int[size]}
	{
		cout << "B is created" << endl;
	}
	
	~B()
	{
		cout << "B is destroyed" << endl;
		if(memory!= nullptr)
		{
			delete []memory;
		}
		
	}
	
	void do_something() override
	{
		cout << "B: do_something()" << endl;
		return;
	}
	
};

void client_function(A* a)
{
	a->do_something(); // calls A' do_something or B's do_something at run time
	
	return;
}

void function_1(void)
{
	//A a(10);
	//
	//a.do_something();
	//
	//a.cleanup();
	//
	//cout << "function_1 ends here" << endl;
	//
	//return;
}

void function_2()
{
	A a(50);
	
	a.do_something();
	
	cout << "function_2 ends here" << endl;
	// let us hope C++ will call the destrcutor at the end of function_2
	
	return;
}

void function_3(void)
{
	A a(50);
	
	try
	{
		a.do_something();
	}
	catch(...)// catch all exceptions 
	{
		cout << "some exception caught" << endl;
	}
	
	cout << "function_2 ends here" << endl;
	
	return;
}

int main()
{
	//function_3();
	B *b = new B(10);
	
	client_function(b);
	
	return 0;
}

