#include <iostream>

using namespace std;

class Queue
{
public:
    enum class error_codes{bad_size = -1};
private:
    int *memory;
    int size_;
    int head;
    int tail;
    bool is_full;
    bool is_empty;
public:
    explicit Queue(const int size = 0) : 
        size_{size}, is_full{false}, is_empty{true}
    {
        if(size_ <= 0)
        {
            cout << "Bad size specified" << endl;
            throw error_codes::bad_size;
        }
        
        memory = new int[size_];
        
        head = 0;
        tail = 0;
        cout << "Queue with size : " << size_ << " is created " << endl;
    }
    
    ~Queue()
    {
        delete []memory;
        head = 0;
        tail = 0;
        cout << "Queue with size : " << size_ << " is destroyed" << endl;
        size_ = 0;
    }
    
    // delete copy/move operations 
    Queue(const Queue& other_queue) =delete;
    Queue& operator=(const Queue& other_queue) =delete;
    Queue(Queue&& other_queue) =delete;
    Queue& operator=(Queue&& other_queue) =delete;
    
    bool insert(const int data)
    {
        bool ret = true;
        
        if( is_full == false )
        {
            memory[tail] = data;
            tail = (tail + 1) % size_;
            if(tail == head)
            {
                is_full = true;
            }
            is_empty = false;
        }
        else
        {
            // queue is full
            cout << "\tQueue is full : " << data  << endl;
            ret = false;
        }
        
        return ret;
    }
    
    bool get(int * data)
    {
        bool ret = true;
        
        if(is_empty == false)
        {
            *data = memory[head];
            head = (head + 1) % size_;
            is_full = false;
            if(head == tail)
            {
                is_empty = true;
            }
        }
        else
        {
            // queue empty
            cout << "\tQueue is empty" << endl;
            ret = false;
        }
        
        return ret;
    }
    
    int size() const
    {
        return size_;
    }
    
};

int main()
{
    const int q1_size = 10;
    Queue q1(q1_size);
    
    for(int count = 0; q1.insert(count); ++count)
    {
        // do nothing
    }
    
    int val = 0;
    
    for(int count = 0; q1.get(&val); ++count)
    {
        
        cout << "Val popped : " << dec << val << endl;
    }
    
    return 0;
}

