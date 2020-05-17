#include <iostream>
#include <thread>
#include <condition_variable>
#include <string>
#include <memory>
#include <atomic>

using namespace std;

template<typename T>
class Queue
{
public:
    enum class error_codes{bad_size = -1};
private:
    T *memory;
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
        
        memory = new T[size_];
        
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
    
    
    bool insert(const T data)
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
    
    bool get(T * data)
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

class logger;
void print(const logger* this_logger);

class logger
{
private:
    static shared_ptr<logger> instance;
    static std::mutex mutex_;
    static bool instance_created;
    
    int size_;
    Queue<std::string> *queue;
    std::thread printer_thread;
    
    // private constructor
    logger() : 
        size_{100},  
        queue( new Queue<std::string>(size_) ),
        printer_thread(print, this)
    {
        cout << "Logger instance created" << endl;
    }
    
public:
    ~logger()
    {
        end_session();
        printer_thread.join();
        delete queue;
        cout << "Logger destroyed" << endl;
    }
    
    static shared_ptr<logger> get_logger_instance()
    {
        unique_lock<std::mutex> lck(mutex_);
        
        if(instance_created == false)
        {
            // creates a logger instance
            instance.reset(new logger());
            instance_created = true;
            lck.unlock();
        }
        else
        {
            cout << "Logger instance already created. Returning instance" << endl;
        }
        
        return instance;
    }
    
    bool log_info(const string& info)
    {
        string temp("INFO : ");
        temp += info;
        return queue->insert(temp);
    }
    
    bool log_error(const string& error)
    {
        string temp("ERROR : ");
        temp += error;
        return queue->insert(temp);
    }
    
    bool end_session()
    {
        string temp("END_LOG");
        return queue->insert(temp);
    }
    
    friend void print(const logger *this_logger);
    
};

std::mutex logger::mutex_;
bool logger::instance_created = false;
shared_ptr<logger> logger::instance(nullptr);

void print(const logger *this_logger)
{
    string message;
    cout << "printer thread spawned" << endl;
    while(true)
    {
        this_logger->queue->get(&message);
        cout << message << endl;
        
        if(0 == message.compare("END_LOG"))
        {
            cout << "Printer thread execution complete" << endl;
            break; 
        }
    }
    
    return;
}

void log_hello(void)
{
    shared_ptr<logger> sh_logger = logger::get_logger_instance();
    logger *logger_instance = sh_logger.get();
    
    for(int count = 0; count < 5; ++count)
    {
        logger_instance->log_info(" Hello ");
    }
    
    return;
}

void log_world(void)
{
    shared_ptr<logger> sh_logger = logger::get_logger_instance();
    logger *logger_instance = sh_logger.get();
    
    for(int count = 0; count < 5; ++count)
    {
        logger_instance->log_info(" World ");
    }
    
    return;
}

class thread_guard
{
private:
    std::thread this_thread;
public:
    explicit thread_guard(void(*th_function)(void)):
        this_thread(th_function)
    {
        // do nothing
    }
    ~thread_guard()
    {
        this_thread.join();
        cout << "Thread_Guard : Thread joined" << endl;
    }
    // delete copy/move operations
    thread_guard(const thread_guard&) =delete;
    thread_guard(thread_guard&&) =delete;
    thread_guard& operator=(const thread_guard&) =delete;
    thread_guard& operator=(thread_guard&&) =delete;
};

int main()
{    
    thread_guard guard_hello_thread(log_hello);
    thread_guard guard_world_thread(log_world);
    
    cout << "Main ends" << endl;
    
    return 0;
}

