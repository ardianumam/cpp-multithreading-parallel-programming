#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>
#include <iostream>

using namespace std;

// concurrent queue
template <class T>
class concurrent_queue_cv {
	std::mutex m;
	std::queue<T> q;
	std::condition_variable cv;
public:
	concurrent_queue_cv() = default;
	void push(T value) {
		std::lock_guard<std::mutex> lg(m);
		q.push(value);
		cv.notify_one();
	}

	void pop(T& value) {
		std::unique_lock<std::mutex> lg(m);
		cv.wait(lg, [this] {return !q.empty();});
		value = q.front();
		q.pop();
	}
};


// thread pool
using Func = std::function<void()>;

class thread_pool {
	concurrent_queue_cv<Func> work_queue;
	std::vector<std::thread> threads;
	
    void worker() { // entry function for each thread. So, each thread has one worker
        while (true) {
            Func task;
            work_queue.pop(task); // pop will block if the queue empty (use condition variable wait: see above pop implementation)
            task(); // this line also blocks for the next instruction, since worker is a function entry to a single thread, thus all instructions here, within the same thread, will be execued sequentially
        }
    }

public:
    // Constructor: Initializes worker threads equal to the hardware concurrency. 
    // Each thread has one entry function, that is void worker. This function do while true,
    // where it continuously take (pop) the task from the work queue.
	thread_pool() {
        const unsigned thread_count = thread::hardware_concurrency();
        for (unsigned i = 0; i < thread_count; ++i)
            threads.push_back(thread{&thread_pool::worker, this});
    }

    // Destructor: Joins all threads before destruction to ensure they complete execution.
	~thread_pool() {
        for (auto& t: threads) {
            t.join();
        }
    }

    // Submits a task to the task queue.
	void submit(Func f) {
        work_queue.push(f);
    }
};


mutex m;

// entry task for the thread
void task() {
	lock_guard<mutex> lg(m);
	cout << "Thread id: " << this_thread::get_id() << " started" << endl;
	this_thread::sleep_for(100ms);
	cout << "Thread id: " << this_thread::get_id() << " finished" << endl;
}

int main() {
	cout << "Creating a thread pool with " << thread::hardware_concurrency() << " threads" << endl;
	thread_pool pool;
	
	for (int i = 0; i < 20; ++i) // user can just submit task to the pool
		pool.submit(task);
	
	this_thread::sleep_for(5s); // we add this to make sure that the main function will not end before all threads get initialized
}