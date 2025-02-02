// wait and block the subsequent instructions till the counter is not zero --> why just dont use predicate involving counter in the wait here???
#include <mutex>
#include <condition_variable>
#include <iostream>

class Semaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int counter{0};
    const int max_count{10}; // default max resources, or we can enable to pass this as argument in the object instantiation
public:
    void release() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Adding one item" << std::endl;

        if (counter < max_count) {
            ++counter;
			count();
		}

        cv.notify_all();
    }

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Removing one item" << std::endl;

        // while (counter == 0) {
        //     cv.wait(lock);
        // }
        cv.wait(lock, [this] { return counter > 0; }); // change to wait with predicate instead of using while true as in the original lecture.

        --counter;
        count();
    }

    void count() const {
        std::cout << "Value of counter: ";
        std::cout << counter << std::endl;
    }
};

#include <iostream>
#include <thread>
#include <vector>

int main() {
    Semaphore sem;

    auto insert = [&sem]() {
        sem.release();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    };

    auto relinquish = [&sem] {
        sem.acquire();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    };

    std::vector<std::thread> tasks;

    for (int i = 0; i < 2; ++i)
        tasks.push_back(std::thread(insert));
    for (int i = 0; i < 4; ++i)
        tasks.push_back(std::thread(relinquish));
    for (int i = 0; i < 3; ++i)
        tasks.push_back(std::thread(insert));
    for (auto& task : tasks) {
        task.join();
    }
}