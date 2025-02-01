// Lazy initialization (multi-threaded)
// Uses the Double-checked Locking algorithm
#include <mutex>
#include <thread>
#include <iostream>


class Test {
	// ...
public:
	void func() { /*...*/ }
};

Test *ptest = nullptr;            // Variable to be lazily initialized
std::mutex mut;

void process() {
    if (!ptest) {                     // First check of ptest
        std::lock_guard<std::mutex> lck_guard(mut);
        
        if (!ptest)                  // Second check of ptest
            ptest = new Test;   // Initialize ptest --> before C++17, this may raise an issue. The pointer is assigned an address but the object is not yet constructed. If another threads use it as it is not nullptr anymore, it triggers an error since the object is not yet constructed
    }
     ptest->func();
}

int main()
{
	std::thread thr1(process);
	std::thread thr2(process);
	thr1.join(); thr2.join();
}
