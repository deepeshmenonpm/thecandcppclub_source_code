#include <iostream>

using namespace std;

// NOTE 1: deleter not supported as of now
// NOTE 2: Single Object version 
template<typename T>
class Unique_Ptr
{
private:
    T* raw_ptr;
public:
    Unique_Ptr(T* object = nullptr) : raw_ptr{object}
    {
        // do nothing
    }
    
    ~Unique_Ptr()
    {
        if(raw_ptr != nullptr)
        {
            delete raw_ptr;
        }
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
    }
    
    Unique_Ptr& operator=(Unique_Ptr&& other_unique_ptr)
    {
        // transfer the ownership of memory 
        raw_ptr = other_unique_ptr.raw_ptr;
        other_unique_ptr.raw_ptr = nullptr;
        
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
            delete old_ptr;
        }
    }
    
    // NOTE: deleter is yet to be implemented
    
};

// utility for making Unique_Ptr
template<typename T, typename... Arglist>
Unique_Ptr<T> make_unique_pointer(Arglist&&... args)
{
    return(Unique_Ptr<T>(new T(std::forward<Arglist>(args)...)));
}


int main()
{
    Unique_Ptr<int> ptr = make_unique_pointer<int>(1);
    
    cout << "*ptr : " << *ptr << endl;
    
    *ptr = 2;
    
    cout << "*ptr : " << *ptr << endl;
    
    class st
    {
    public:
        int i_;
        int j_;
        int k_;
        st(int i = 0, int j = 0, int k = 0): i_{i}, j_{j}, k_{k} {}
    };
    
    cout << "\nTesting Unique_Ptr on class st..." << endl;
    Unique_Ptr<st> ptr_st = make_unique_pointer<st>(1, 2, 3); 
    
    cout << "ptr_st->k_ : " << ptr_st->k_ << endl;
    cout << "ptr_st->j_ : " << ptr_st->j_ << endl;
    cout << "ptr_st->i_ : " << ptr_st->i_ << endl;
    
    return 0;
}