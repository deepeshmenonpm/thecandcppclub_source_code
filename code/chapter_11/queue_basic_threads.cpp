#include <iostream>
#include <thread>
#include <condition_variable>

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
    std::mutex mutex_;
    std::condition_variable cond_var;
    bool data_ready;
public:
    explicit Queue(const int size = 0) : 
        size_{size}, data_ready{false}, is_full{false}, is_empty{true}
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
        std::unique_lock<mutex> lck_(mutex_);
        
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
        
        if(ret == true)
        {
            data_ready = true;
            cond_var.notify_one(); // unlocks the lock within the function 
        }
        
        //lck_.unlock(); // actually unnecessary to do this for unique_lock 
        
        return ret;
    }
    
    bool get(int * data)
    {
        bool ret = true;
        
        unique_lock<mutex> lck(mutex_);
        
        // predicate 'check for data_ready' is VERY IMPORTANT here 
        cond_var.wait(lck, [this]{return this->data_ready;} );
        
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

void producer(Queue& queue)
{
    bool ret = false;
    
    for(int count = 0; count < 100; ++count)
    {
        while(false == queue.insert(count));            
    }
    
    return;
}


void consumer(Queue& queue)
{
    int val = 0;
    while(true)
    {
        queue.get(&val);
        
        cout << "Val popped : " << val << endl;
        
        if(val == 99) break;
    }
    
    return;
}


int main()
{    
    Queue q1(10);
    
    // create consumer thread first 
    std::thread consumer_thread(consumer, std::ref(q1));
    
    // create producer thread
    std::thread producer_thread(producer, std::ref(q1));

    // wait for the threads to join back main
    producer_thread.join();
    consumer_thread.join();
    
    cout << "Main ends" << endl;
    
    return 0;
}

