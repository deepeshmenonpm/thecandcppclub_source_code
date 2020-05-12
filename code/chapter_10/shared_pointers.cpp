#include <iostream>
#include <atomic>

using namespace std;

// single object deleter version 
template<typename T>
struct default_deleter // default_deleter is a functor or function object 
{    
    // don't have any private/public/protected data 
    void operator()(T* ptr)
    {
        if(ptr != nullptr) // this check is very important 
        {
            cout << "default_deleter" << endl;
            // default deleter is as good as a 
            // direct delete on the pointer. 
            // There cannot be any custom cleanup here.  
            delete ptr;
        }
    }
};

// Array deleter version
template<typename T>
struct default_deleter<T[]> // default_deleter is a functor or function object 
{    
    // don't have any private/public/protected data 
    void operator()(T* ptr)
    {
        if(ptr != nullptr) // this check is very important 
        {
            cout << "default_deleter [] " << endl;
            // default deleter is as good as a 
            // direct delete on the pointer. 
            // There cannot be any custom cleanup here.  
            delete []ptr;
        }
    }
};

// An example of default template type. 
// TP's default 'type' is default_deleter<T>
// If provided, a custom deleter TP, user can either 
// install a functor object or it can be a simple
// function pointer which accepts a pointer parameter. 
template< typename T, typename TP = default_deleter<T> >
class Shared_Ptr
{
private:
    std::atomic<int*> reference_count;
    T* raw_ptr;
    TP deleter; // will get constructed anyways with default_deleter type  
public:
    explicit Shared_Ptr(T* object) : 
        raw_ptr{object}, 
        reference_count(new int(1)) // cannot use {}, use ()
    {
        cout << "Shared_Ptr created with Ref count : " << *reference_count << endl;
    }
    
    // constructor which accepts a custom deleter 
    Shared_Ptr(T* object, TP custom_deleter) : 
        raw_ptr{object}, 
        deleter{custom_deleter}, 
        reference_count(new int(1)) // cannot use {}, use ()
    {
        cout << "Shared_Ptr created with Ref count : " << *reference_count << endl;
    }
    
    ~Shared_Ptr()
    {
        if(reference_count != nullptr)
        {
            --(*reference_count);
            cout << "In Shared_Ptr Destructor Ref count : " << *reference_count << endl;
            if(*reference_count <= 0)
            {
                if(raw_ptr != nullptr)
                {
                    // call deleter
                    deleter(raw_ptr);
                }
            }
        }
    }
    
    TP& get_deleter()
    {
        return deleter;
    }
    
    // define copy operations
    Shared_Ptr(const Shared_Ptr& other_shared_ptr)
    {
        // do not create a new reference_count, copy the pointer from other_shared_ptr
        reference_count = other_shared_ptr.reference_count.load();
        if(reference_count != nullptr)
        {
            ++(*reference_count);
            cout << "In Shared_Ptr Copy Constructor Ref count : " << *reference_count << endl;
        }
        raw_ptr = other_shared_ptr.raw_ptr;
        deleter = other_shared_ptr.deleter;
    }
    
    Shared_Ptr& operator=(const Shared_Ptr& other_shared_ptr)
    {
        // do not create a new reference_count, copy the pointer from other_shared_ptr
        reference_count = other_shared_ptr.reference_count.load();
        if(reference_count != nullptr)
        {
            ++(*reference_count);
            cout << "In Shared_Ptr Copy =operator Ref count : " << *reference_count << endl;
        }
        raw_ptr = other_shared_ptr.raw_ptr;
        deleter = other_shared_ptr.deleter;
        
        return *this;
    }
    
    // Define move operations
    Shared_Ptr(Shared_Ptr&& other_shared_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_shared_ptr.raw_ptr;
        other_shared_ptr.raw_ptr = nullptr;
        reference_count = other_shared_ptr.reference_count.load();
        other_shared_ptr.reference_count = nullptr;
        
        deleter = other_shared_ptr.deleter;        
    }
    
    Shared_Ptr& operator=(Shared_Ptr&& other_shared_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_shared_ptr.raw_ptr;
        other_shared_ptr.raw_ptr = nullptr;
        reference_count = other_shared_ptr.reference_count.load();
        other_shared_ptr.reference_count = nullptr;
        
        deleter = other_shared_ptr.deleter;
        
        return *this;
    }
    
    // define basic operations supported by a regular pointer
    // 1. dereference operation
    T& operator*() const 
    {
        return *raw_ptr;
    }
    
    // 2. member selection operation
    T* operator->() const 
    {
        return raw_ptr;
    }
    
    // 3. indexing operation ( ONLY for array version ) 
    /*T& operator[](const int index)
    {
        return raw_ptr[index];
    }*/
    
    // 4. equality check 
    bool operator==(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr == other_shared_ptr.raw_ptr);
    }
    
    // 5. in-equality check 
    bool operator!=(const Shared_Ptr& other_shared_ptr) const 
    {
        return!(this->operator==(other_shared_ptr));
    }
    
    // 6. less than 
    bool operator<(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr < other_shared_ptr.raw_ptr);
    }
    
    // 7. greater than 
    bool operator>(const Shared_Ptr& other_shared_ptr) const 
    {
        return!(this->operator<(other_shared_ptr));
    }    
    
    // 8. less than or equal  
    bool operator<=(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr <= other_shared_ptr.raw_ptr);
    }
    
    // 9. greater than or equal 
    bool operator>=(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr >= other_shared_ptr.raw_ptr);
    }    
    
    T* get() const 
    {
        return raw_ptr;
    }
    
    explicit operator bool() const 
    {
        return (raw_ptr != nullptr);
    }
    
    T* release()
    {
        T* temp = raw_ptr;
        raw_ptr = nullptr;
        reference_count = nullptr;
        
        return temp;
    }
    
    void reset(T* new_ptr)
    {
        T* old_ptr =  raw_ptr;
        raw_ptr = new_ptr;
        reference_count = 1; // reset back to 1
        
        if(old_ptr != nullptr)
        {
            deleter(old_ptr); 
        }
    }
    
    int use_count() const 
    {
        return *reference_count;
    }
    
};

// Array version T[] with custom deleter support 
template< typename T, typename TP > // Here TP already has the default type as default_deleter<T>
class Shared_Ptr<T[], TP> // Must have Shared_Ptr<T> already defined for T[] to work 
{
private:
    std::atomic<int*> reference_count;
    T * raw_ptr; // pointer to an array of T's
    TP deleter;
public:
    Shared_Ptr(T *object) : 
        raw_ptr{object}, 
        deleter{default_deleter<T[]>()}, // Here we copy-construct specialized deleter<T[]>
        reference_count(new int(1)) // cannot use {}, use ()
    {
        cout << "Shared_Ptr [] created with Ref count : " << *reference_count << endl;
    }
    
    Shared_Ptr(T* object, TP this_deleter): 
        raw_ptr{object}, deleter{this_deleter}, 
        reference_count(new int(1)) // cannot use {}, use ()
    {
        cout << "Shared_Ptr [] created with Ref count : " << *reference_count << endl;
    }
    
    ~Shared_Ptr()
    {
        if(reference_count != nullptr)
        {
            --(*reference_count);
            cout << "Shared_Ptr [] destructor Ref count : " << *reference_count << endl;
            if(*reference_count == 0)
            {
                if(raw_ptr != nullptr)
                {
                    deleter(raw_ptr);
                }
            }
        }
    }
    
    TP& get_deleter()
    {
        return deleter;
    }
    
    // define copy operations
    Shared_Ptr(const Shared_Ptr& other_shared_ptr)
    {
        reference_count = other_shared_ptr.reference_count.load();
        raw_ptr = other_shared_ptr.raw_ptr;
        
        if(raw_ptr != nullptr)
        {
            ++(*reference_count);
        }
        
        deleter = other_shared_ptr.deleter;
        
        cout << "In Shared_Ptr [] Copy constructor Ref count : " << *reference_count << endl;
        
    }
    
    Shared_Ptr& operator=(const Shared_Ptr& other_shared_ptr)
    {
        reference_count = other_shared_ptr.reference_count.load();
        raw_ptr = other_shared_ptr.raw_ptr;
        
        if(raw_ptr != nullptr)
        {
            ++(*reference_count);
        }
        
        deleter = other_shared_ptr.deleter;
        
        cout << "In Shared_Ptr [] Copy =operator Ref count : " << *reference_count << endl;
        
        return *this;
    }
    
    // Define move operations
    Shared_Ptr(Shared_Ptr&& other_shared_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_shared_ptr.raw_ptr;
        other_shared_ptr.raw_ptr = nullptr;
        
        deleter = other_shared_ptr.deleter; 
        
        reference_count = other_shared_ptr.reference_count.load();
        other_shared_ptr.reference_count = nullptr;
        
    }
    
    Shared_Ptr& operator=(Shared_Ptr&& other_shared_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_shared_ptr.raw_ptr;
        other_shared_ptr.raw_ptr = nullptr;
        
        deleter = other_shared_ptr.deleter; 
        
        reference_count = other_shared_ptr.reference_count.load();
        other_shared_ptr.reference_count = nullptr;
        
        return *this;
    }
    
    // define basic operations supported by a regular pointer
    // 1. dereference operation
    T& operator*() const 
    {
        return *raw_ptr;
    }
    
    // 2. member selection operation
    T* operator->() const 
    {
        return raw_ptr;
    }
    
    // 3. indexing operation ( ONLY for array version ) 
    T& operator[](const int index)
    {
        return raw_ptr[index];
    }
    
    // 4. equality check 
    bool operator==(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr == other_shared_ptr.raw_ptr);
    }
    
    // 5. in-equality check 
    bool operator!=(const Shared_Ptr& other_shared_ptr) const 
    {
        return!(this->operator==(other_shared_ptr));
    }
    
    // 6. less than 
    bool operator<(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr < other_shared_ptr.raw_ptr);
    }
    
    // 7. greater than 
    bool operator>(const Shared_Ptr& other_shared_ptr) const 
    {
        return!(this->operator<(other_shared_ptr));
    }    
    
    // 8. less than or equal  
    bool operator<=(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr <= other_shared_ptr.raw_ptr);
    }
    
    // 9. greater than or equal 
    bool operator>=(const Shared_Ptr& other_shared_ptr) const 
    {
        return(raw_ptr >= other_shared_ptr.raw_ptr);
    }    
    
    T* get() const 
    {
        return raw_ptr;
    }
    
    explicit operator bool() const 
    {
        return (raw_ptr != nullptr);
    }
    
    T* release()
    {
        T* temp = raw_ptr;
        raw_ptr = nullptr;
        reference_count = nullptr;
        
        return temp;
    }
    
    void reset(T* new_ptr)
    {
        T* old_ptr =  raw_ptr;
        raw_ptr = new_ptr;
        reference_count = 1; // reset back to 1
        
        if(old_ptr != nullptr)
        {
            deleter(old_ptr);
        }
    }
    
    int use_count() const 
    {
        return *reference_count;
    }
    
};

// A custom deleter 
struct Custom_Deleter
{
    void operator()(int* ptr)
    {
        if(ptr != nullptr)
        {
            cout << "Custom deleter" << endl;
            // do some other cleanup if any
            delete ptr;
        }
    }
};

// A generic template based custom deleter 
template<typename G> // G for generic :) (kidding)
struct Generic_Custom_Deleter
{
private:
    // NOTE: The count_ is just used to identify the 
    // Generic_Custom_Deleter that is called 
    // when a user changes the deleter after 
    // the Shared_Ptr has been already created
    int count_;
public:
    Generic_Custom_Deleter(int count = 0): count_{count} {}
    void operator()(G* ptr)
    {
        if(ptr != nullptr)
        {
            cout << "Generic Custom deleter - " << count_ << endl;
            // do some other cleanup if any
            delete ptr;
        }
    }
};

// A function type deleter 
void deleter_function(int* ptr)
{
    if(ptr != nullptr)
    {
        cout << "Function deleter" << endl;
        // do some other cleanup if any
        delete ptr;
    }
    
    return;
}

struct Custom_Array_Deleter
{
    void operator()(int *ptr)
    {
        if(ptr != nullptr)
        {
            cout << "Custom_Array_Deleter" << endl;
            delete[] ptr; // see the array version of delete[] used here 
        }
        
        return;
    }
};

int main()
{   
    {
        Shared_Ptr<int> sh_ptr1(new int); 
        // local scope 1
        {
            Shared_Ptr<int> sh_ptr2 = sh_ptr1; // copy construction 
            *sh_ptr2 = 5;
        }
        
        cout << "Outside local scope 1" << endl;
        cout << "*shptr1 : " << *sh_ptr1 << endl;
    
    }
    
    cout << endl;
    
    {
        const int size = 10;
        Shared_Ptr<int[]> sh_ptr3(new int[size]); 
        // local scope 2
        {
            Shared_Ptr<int[]> sh_ptr4 = sh_ptr3; // copy construction 
            for(int index = 0; index < size; ++index)
            {
                sh_ptr4[index] = index*10;
            }
        }
        
        cout << "Outside local scope 2" << endl;
        
        for(int index = 0; index < size; ++index)
        {
            cout << "sh_ptr3[" << index << "] : " << sh_ptr3[index] << endl;
        }
    }
    
    return 0;
}