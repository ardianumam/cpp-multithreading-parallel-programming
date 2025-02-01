// First program with std::thread
#include <thread>
#include <iostream>

// Callable object - thread entry point
void hello() {
	std::cout << "Hello, Thread!\n";
}

int main() {
	// Create an std::thread object
	// Pass the entry point function, in this case is hello, to the constructor
	std::thread thr(hello);
	
	// The thread parent, that is the main function, wait for the thread to complete
	thr.join();
}