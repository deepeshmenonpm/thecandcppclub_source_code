#include <iostream>

using namespace std;

template <typename T>
class Array
{
public:
    enum class error_code{bad_size = -1, bad_index = -2};
private:
    T *memory;
    int size_;
public:
    explicit Array(int size)
    {
        // RAII class invariant check 
        if(size <= 0)
        {
            cout << "Bad size" << endl;
            throw error_code::bad_size;
        }
        
        size_= size;
        memory = new T[size_];
        
        cout << "Array is created with size " << size_ << endl;
    }
    
    ~Array()
    {
        cout << "Array is destroyed" << endl;
        
        if(memory != nullptr)
        {
            delete []memory;
        }
        
    }
    
    // copy constructor
    Array(const Array& other_object)
    {
        cout << "Array object's clone is created" << endl;
        size_ = other_object.size_;
        memory = new T[size_];
        
        // start copying byte by byte
        for(int index = 0; index < size_; ++index)
        {
            memory[index] = other_object.memory[index];
        }
    }
    
    // copy assignment operator 
    Array& operator=(const Array& other_object)
    {
        if(this == &other_object)
        {
            return *this;
        }
        
        cout << "Array object is being cloned" << endl;
        
        if(size_ < other_object.size_)
        {
            delete []memory;
            memory = new T[other_object.size_];
        }
        
        size_ = other_object.size_;

        // Copy byte by byte to memory
        for(int index = 0; index < size_; ++index)
        {
            memory[index] = other_object.memory[index];
        }
        
        return *this;
    }
    
    // move constructor
    Array(Array &&other_object)
    {
        cout << "Move constructor: Array has been moved!" << endl;
        size_ = other_object.size_;
        memory = other_object.memory;
        
        //destroy the contents of other_object
        other_object.size_ = 0;
        other_object.memory = nullptr;
    }
    
    // move assignment operator
    Array& operator=(Array&& other_object)
    {
        cout << "An Array object is moved to this Array object" << endl;
        if(this == &other_object)
        {
            return *this;
        }
        
        size_ = other_object.size_;
        delete []memory; // so important. Release the old memory to heap
        memory = other_object.memory;
        
        // destory the contents of other object
        other_object.size_ = 0;
        other_object.memory = nullptr;
        
        return *this;
    }
    
    // indexing operator 
    T& operator[](const int index)
    {
        if(   ( index >= size_ ) 
            ||( index < 0 ) )
        {
            cout << "Bad index" << endl;
            throw error_code::bad_index;
        }
        
        return memory[index];
    }
    
    int size() const
    {
        return size_;
    }
    
};


struct st
{
	int i;
	char string[10];
	
	st& operator=(const st& st_rhs)
	{
		int index = 0;
		i = st_rhs.i;
		
		for(char c: st_rhs.string)
		{
			string[index++] = c;
		}
	}
};

int main()
{
    Array<int> a(10);
    
    for(int index = 0; index < a.size(); ++index)
    {
        a[index] = index;
    }
    
    for(int index = 0; index < a.size(); ++index)
    {
        cout << "a[" << index << "] : " << a[index] << endl;
    }
    
    cout << "\nTesting Array<double>...\n" << endl; 
    Array<double> d(5);
    double number;
    
    d[0] = 1.25F;
    d[1] = 2.278F;
    d[2] = -5.0F;
    d[3] = 100.798F;
    d[4] = 10.2F;
    
    for(int index = 0; index < d.size(); ++index)
    {
        cout << "d[" << index << "] : " << d[index] << endl;
    }
	
	cout << "\nTesting Array<struct st>...\n" << endl;
	
	Array<st> s(2);
	st temp = {1, "Hello"};
	s[0] = temp;
	
	temp.i = 2;
	
	const char *const str = "World";
	int i = 0;
	for(i = 0; str[i]; ++i)
	{
		temp.string[i] = str[i];
	}
	temp.string[i] = '\0';
	
	s[1] = temp;
	
	for(int index = 0; index < s.size(); ++index)
    {
        cout << "s[" << index << "].i : " << s[index].i << endl;
        cout << "s[" << index << "].string : " << s[index].string << endl;
    }
	
	cout << endl;
    
    return 0;
}
