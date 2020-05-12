#include <iostream>

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
class Unique_Ptr
{
private:
    T* raw_ptr;
    TP deleter; // will get constructed anyways with default_deleter type  
public:
    explicit Unique_Ptr(T* object) : raw_ptr{object}
    {
        // do nothing 
    }
    
    // constructor which accepts a custom deleter 
    Unique_Ptr(T* object, TP custom_deleter) : 
        raw_ptr{object}, deleter{custom_deleter}
    {
        // do nothing 
    }
    
    ~Unique_Ptr()
    {
        if(raw_ptr != nullptr)
        {
            // call deleter
            deleter(raw_ptr);
        }
    }
    
    TP& get_deleter()
    {
        return deleter;
    }
    
    // delete copy operations
    Unique_Ptr(const Unique_Ptr& other_unique_ptr) =delete;
    Unique_Ptr& operator=(const Unique_Ptr& other_unique_ptr) =delete;
    
    // Define move operations
    Unique_Ptr(Unique_Ptr&& other_unique_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_unique_ptr.raw_ptr;
        other_unique_ptr.raw_ptr = nullptr;
        
        deleter = other_unique_ptr.deleter;        
    }
    
    Unique_Ptr& operator=(Unique_Ptr&& other_unique_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_unique_ptr.raw_ptr;
        other_unique_ptr.raw_ptr = nullptr;
        
        deleter = other_unique_ptr.deleter;
        
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
    bool operator==(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr == other_unique_ptr.raw_ptr);
    }
    
    // 5. in-equality check 
    bool operator!=(const Unique_Ptr& other_unique_ptr) const 
    {
        return!(this->operator==(other_unique_ptr));
    }
    
    // 6. less than 
    bool operator<(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr < other_unique_ptr.raw_ptr);
    }
    
    // 7. greater than 
    bool operator>(const Unique_Ptr& other_unique_ptr) const 
    {
        return!(this->operator<(other_unique_ptr));
    }    
    
    // 8. less than or equal  
    bool operator<=(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr <= other_unique_ptr.raw_ptr);
    }
    
    // 9. greater than or equal 
    bool operator>=(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr >= other_unique_ptr.raw_ptr);
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
        
        return temp;
    }
    
    void reset(T* new_ptr)
    {
        T* old_ptr =  raw_ptr;
        raw_ptr = new_ptr;
        
        if(old_ptr != nullptr)
        {
            deleter(old_ptr); 
        }
    }
    
};

// Array version T[] with custom deleter support 
template< typename T, typename TP > // Here TP already has the default type as default_deleter<T>
class Unique_Ptr<T[], TP> // Must have Unique_Ptr<T> already defined for T[] to work 
{
private:
    T * raw_ptr; // pointer to an array of T's
    TP deleter;
public:
    Unique_Ptr(T *object) : 
        raw_ptr{object}, deleter{default_deleter<T[]>()} // Here we copy-construct specialized deleter<T[]>
    {
        // do nothing 
    }
    
    Unique_Ptr(T* object, TP this_deleter): raw_ptr{object}, deleter{this_deleter}
    {
        // do nothing 
    }
    
    ~Unique_Ptr()
    {
        if(raw_ptr != nullptr)
        {
            //delete []raw_ptr; // see the array version of delete
            deleter(raw_ptr);
        }
    }
    
    TP& get_deleter()
    {
        return deleter;
    }
    
    // delete copy operations
    Unique_Ptr(const Unique_Ptr& other_unique_ptr) =delete;
    Unique_Ptr& operator=(const Unique_Ptr& other_unique_ptr) =delete;
    
    // Define move operations
    Unique_Ptr(Unique_Ptr&& other_unique_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_unique_ptr.raw_ptr;
        other_unique_ptr.raw_ptr = nullptr;
        
        deleter = other_unique_ptr.deleter; 
    }
    
    Unique_Ptr& operator=(Unique_Ptr&& other_unique_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_unique_ptr.raw_ptr;
        other_unique_ptr.raw_ptr = nullptr;
        
        deleter = other_unique_ptr.deleter; 
        
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
    bool operator==(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr == other_unique_ptr.raw_ptr);
    }
    
    // 5. in-equality check 
    bool operator!=(const Unique_Ptr& other_unique_ptr) const 
    {
        return!(this->operator==(other_unique_ptr));
    }
    
    // 6. less than 
    bool operator<(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr < other_unique_ptr.raw_ptr);
    }
    
    // 7. greater than 
    bool operator>(const Unique_Ptr& other_unique_ptr) const 
    {
        return!(this->operator<(other_unique_ptr));
    }    
    
    // 8. less than or equal  
    bool operator<=(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr <= other_unique_ptr.raw_ptr);
    }
    
    // 9. greater than or equal 
    bool operator>=(const Unique_Ptr& other_unique_ptr) const 
    {
        return(raw_ptr >= other_unique_ptr.raw_ptr);
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
        
        return temp;
    }
    
    void reset(T* new_ptr)
    {
        T* old_ptr =  raw_ptr;
        raw_ptr = new_ptr;
        
        if(old_ptr != nullptr)
        {
            delete old_ptr;
        }
    }
    
};

// Use this variant of make_unique_ptr for single object creation
template<typename T, typename... Arglist>
Unique_Ptr<T> make_unique_ptr(Arglist&&... args)
{
    return Unique_Ptr<T>(new T(std::forward<Arglist>(args))...);
}

// Use this variant of make_unique_ptr for array of data/objects 
template<typename T, int N>
Unique_Ptr<T[]> make_unique_ptr()
{
    return Unique_Ptr<T[]>(new T[N]);
}

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
    // the Unique_Ptr has been already created
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
    // local scope 1
    {    
        // uses default deleter
        Unique_Ptr<int> int_ptr_1 = make_unique_ptr<int>(30);
        cout << "\n*int_ptr_1 : " << dec << *int_ptr_1 << endl;    
    }
    
    cout << "\nEnd of local scope 1" << endl;
    
    // local scope 2
    {
        Custom_Deleter cd;
        
        Unique_Ptr<int, Custom_Deleter> int_ptr_2( new int(50), cd );
        
        cout << "*int_ptr_2 : " << *int_ptr_2 << endl;
    }
    
    cout << "\nEnd of local scope 2" << endl;
    
    // local scope 3
    {
        Generic_Custom_Deleter<int> gcd;
        
        Unique_Ptr<int, Generic_Custom_Deleter<int>> int_ptr3(new int(60), gcd);
        
        cout << "*int_ptr3 : " << *int_ptr3 << endl;
        
        Generic_Custom_Deleter<int> gcd_2(2);
        int_ptr3.get_deleter() = gcd_2;
        
    }
    
    cout << "\nEnd of local scope 3" << endl;
    
    // local scope 4
    {
        Unique_Ptr<int, void(*)(int *)> int_ptr4(new int(70), deleter_function);
        cout << "*int_ptr3 : " << *int_ptr4 << endl;
    }
    
    cout << "\nEnd of local scope 4" << endl;
    
    // local scope 5
    {
        const int size = 10;
        Unique_Ptr<int[]> int_ptr5 = Unique_Ptr<int[]>(new int[size]);
        for(int index = 0; index < size; ++index)
        {
            int_ptr5[index] = index;
        }
        
        for(int index = 0; index < size; ++index)
        {
            cout << "int_ptr5[" << index << "]: " << int_ptr5[index] << endl;
        }
        
    }
    
    cout << "\nEnd of local scope 5" << endl;
    
    // local scope 6
    {
        const int size = 10;
        Custom_Array_Deleter cad;
        
        Unique_Ptr<int[], Custom_Array_Deleter> int_ptr6 = // Lenghty line 
            Unique_Ptr<int[], Custom_Array_Deleter>(new int[size], cad);
        
        for(int index = 0; index < size; ++index)
        {
            int_ptr6[index] = index;
        }
        
        for(int index = 0; index < size; ++index)
        {
            cout << "int_ptr6[" << index << "]: " << int_ptr6[index] << endl;
        }
        
    }
    
    cout << "\nEnd of local scope 6" << endl;
    
    return 0;
}