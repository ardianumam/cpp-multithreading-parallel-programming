// Simple concurrent queue implementation
// with condition variable
// pop() blocks when the queue is empty
// push() polls when the queue is full
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <future>
#include <string>

using namespace std::literals;

// Concurrent queue class
template <class T>
class concurrent_queue_cv {
    // Data members
    std::mutex mut;
    std::queue<T> que;
    std::condition_variable not_full, not_empty;
    int max{50};
public:
    // Constructors
    concurrent_queue_cv() = default;
    concurrent_queue_cv(int max) : max(max) {}
    
    // Delete unnecessary member functions
    concurrent_queue_cv(const concurrent_queue_cv&) = delete;
    concurrent_queue_cv& operator=(const concurrent_queue_cv&) = delete;
    concurrent_queue_cv(concurrent_queue_cv&&) = delete;
    concurrent_queue_cv& operator=(concurrent_queue_cv&&) = delete;
    
    // Member functions
    // Push an element onto the queue
    void push(T value) {
        std::unique_lock<std::mutex> uniq_lck(mut);
        
        // Wait until the queue is not full
        not_full.wait(uniq_lck, [this] { return que.size() < max; });
        
        // Perform the push and notify
        que.push(value);
        not_empty.notify_one();
    }
    
    // Pop an element from the queue
    void pop(T& value) {
        std::unique_lock<std::mutex> uniq_lck(mut);
        
        // Block when the queue is empty
        not_empty.wait(uniq_lck, [this] { return !que.empty(); });
        
        // Perform the pop
        value = que.front();
        que.pop();
        not_full.notify_one();
    }
};


concurrent_queue_cv<std::string> conc_que;
    
// Waiting thread
void reader() {
    std::string data;
    
    // Pop some elements from the queue
    std::cout << "Reader calling pop..." << std::endl;
    for (int i = 0; i < 60; ++i) {
        conc_que.pop(data);                                    // Pop the data off the queue
        std::cout << "Reader received data: " << data << std::endl;
    }
}

// Modifying thread
void writer() {
    using namespace std::literals;
    std::this_thread::sleep_for(2s);                       // Pretend to be busy...
    std::cout << "Writer calling push..." << std::endl;
    
    // Push the data onto the queue
    for (int i = 0; i < 60; ++i) {
        std::string data = "Item " + std::to_string(i);
        conc_que.push(data);
    }
    
    std::cout << "Writer returned from push..." << std::endl;
}

int main() {
    // Start the threads
    std::cout << "Starting reader" << std::endl;
    auto read_fut = async(std::launch::async, reader);
    std::cout << "Starting writer" << std::endl;
    auto write_fut = std::async(std::launch::async, writer);
    
    // Wait for them to complete
    read_fut.wait();
    write_fut.wait();
}
