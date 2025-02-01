// Lazy initialization (multi-threaded)
// Inefficient (always locks the mutex)
#include <mutex>
#include <thread>
#include <iostream>


class Test {
	// ...
public:
	void func() { /*...*/ }
};

Test *ptest = nullptr;         // Variable to be lazily initialized
std::mutex mut;

void process() {
    std::unique_lock<std::mutex> uniq_lck(mut);       // Protect ptest

    if (!ptest)
        ptest = new Test;
    uniq_lck.unlock();
    ptest->func();
}

int main()
{
	std::thread thr1(process);
	std::thread thr2(process);
	thr1.join(); thr2.join();
}
