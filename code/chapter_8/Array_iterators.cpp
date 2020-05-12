#include <iostream>
#include <initializer_list>

using namespace std;

template<typename T>
class Array
{
public:
    enum class error_code{bad_size = -1, bad_index = -2};
private:
    T *memory;
    int size_;
public:
    explicit Array( const int size )
    {
        // RAII class invariant check 
        if(size <= 0)
        {
            cout << "Error : Size must be greater than 0" << endl;
            throw(error_code::bad_size);
        }
        size_= size;
        memory = new T[size_];
        
        cout << "Array is created with size " << size_ << endl;
    }
    
    Array(std::initializer_list<T> list) : 
        size_{static_cast<int>(list.size())}, memory{new T[list.size()]}
    {
        // start copying the data from list one by one to Array's memory
        int index = 0;
        for( T val : list)
        {
            memory[index++] = val;
        }
        
        cout << "Array initialized with initializer_list for " << size_ << " members" << endl;
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
    
private:
    // struct 'itr' is used for storing iterator values 
    struct itr
    {
        int current_position;
        Array<T> *this_object;
        itr(const int position, Array<T> *object) : 
            current_position{position}, this_object{object}
        {
            // do nothing 
        }
    };
    
public:
    // Array class supports limited random access type iterator features 
    class iterator
    {
    private:
        itr *this_itr;
    public:
        iterator(const int position = 0, Array<T> *object = nullptr) :
            this_itr{new itr(position, object)}
        {
            // do nothing 
            cout << "itr created" << endl;
        }
        
        ~iterator()
        {
            if(this_itr != nullptr)
            {
                cout << "itr destroyed" << endl;
                delete this_itr;
            }
        }
        
        // copy is not allowed for iterator object 
        iterator(const iterator& other) =delete;
        
        // move constructor for iterator object
        iterator(iterator&& other)
        {
            this_itr = other.this_itr;
            other.this_itr = nullptr;
        }
        
        // copy is not allowed for iterator object 
        iterator& operator=(const iterator& other) =delete;
        
        // move assignment for iterator object
        iterator& operator=(iterator&& other)
        {
            this_itr = other.this_itr;
            other.this_itr = nullptr;
            
            return *(this);
        }
        
        // iterator indexing operation 
        T& operator[](const int index)
        {
            return this_itr->this_object->memory[index];
        }
        
        // iterator inequality check operation 
        bool operator!=(const iterator& other) const 
        {
            bool ret = false;
            if(this_itr->current_position != other.this_itr->current_position)
            {
                ret = true;
            }
            
            return ret;
        }
        
        // iterator equality check operation 
        bool operator==(const iterator& other) const 
        {
            return!(this->operator!=(other));
        }
        
        // iterator increment operation analogous to pointer increment
        iterator& operator++()
        {
            this_itr->current_position++;
            return *this;
        }
        
        // iterator decrement operation analogous to pointer decrement
        iterator& operator--()
        {
            this_itr->current_position--;
            return *this;
        }
        
        // iterator dereference operation analogous t pointer dereference 
        T& operator*()
        {
            return this_itr->this_object->memory[this_itr->current_position];
        }
        
        // iterator less than comparison
        bool operator<(const iterator& other) const
        {            
            return (this_itr->current_position < other.this_itr->current_position)? true : false;
        }
        
        // iterator greater than comparison
        bool operator>(const iterator& other) const 
        {
            return !(this->operator<(other));
        }
        
    };
    
    iterator begin()
    {
        return (iterator(0, this)); // return a temp value ( r-value )
    }
    
    iterator end()
    {
        return (iterator(size_, this)); // return a temp value ( r-value )
    }
    
};


int main()
{
    Array<int> array = {1, 2, 3, 4, 5};
    
    int i = 0;
    
    for(int val : array)
    {
        cout << "array[" << dec << i << "] : " << val << endl;
    }
    
    return 0;
    
}/*
    
    Array<int> int_array(10);
    
    cout << "1 : array initialization..." << endl;
    int index = 0;
    for(int &i : int_array)
    {
        i = index++;
    }
    cout << "2: array initialization done!\n" << endl;
    
    cout << "3 : array read..." << endl;
    for(int i : int_array)
    {
        cout << "i: " << i << endl;
    }
    cout << "4 : array read done!\n" << endl;
    
    cout << "5: explicit iterator syntax operations to set array values..." << endl;
    
    index = 0;
    
    // Now let us see '*', '++' and '<' in action with explcit iterator syntax
    Array<int>::iterator itr1 = int_array.begin();
    Array<int>::iterator itr1_end = int_array.end();
    
    for( ; itr1 < itr1_end; ++itr1 )
    {
        *itr1 = index++;
    }
    
    cout << "\n6: explicit iterator syntax operations to read array values..." << endl;
    
    Array<int>::iterator itr2 = int_array.begin();
    Array<int>::iterator itr2_end = int_array.end();
    for( ; itr2 < itr2_end; ++itr2 )
    {
        cout << "*itr2 : " << dec << *itr2 << endl;
    }
    
    cout << "main ends here " << endl;
    
    return 0;
   
}*/
