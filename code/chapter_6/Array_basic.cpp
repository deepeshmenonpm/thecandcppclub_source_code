#include <iostream>

using namespace std;

class Array
{
public:
    enum class error_code{bad_size = -1, bad_index = -2};
private:
    int *memory;
    int size_;
public:
    explicit Array(int size)
    {
        if(size <= 0)
        {
            cout << "Bad size" << endl;
            throw error_code::bad_size;
        }
        
        size_= size;
        memory = new int[size_];
        
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
        memory = new int[size_];
        
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
            memory = new int[other_object.size_];
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
    int& operator[](const int index)
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

int main()
{
    Array a(20);
    
    for(int index = 0; index < a.size(); ++index)
    {
        a[index] = index;
    }
    
    for(int index = 0; index < a.size(); ++index)
    {
        cout << "a[" << index << "] : " << a[index] << endl;
    }
    
    return 0;
}
