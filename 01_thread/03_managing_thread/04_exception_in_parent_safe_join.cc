// Safely destroying an std::thread object
// when an exception is thrown (verbose)
#include <thread>
#include <iostream>

// Callable object - thread entry point
void hello()
{
	std::cout << "Hello, Thread!\n";
}

int main()
{
	// Create an std::thread object outside the exception scope
	std::thread thr(hello);
	
	try {
		// Code that might throw an exception
		throw std::exception();

		// If no exception we got here, call join() as usual
		thr.join();
	}
	catch (std::exception& e) {
		std::cout << "Exception caught: " << e.what() << '\n';
		thr.join(); // Call join() before thr's destructor is called in the next scope, so, this is safe because the parent will wait the thread finishes and not call terminate
	}
	// Calls ~thr() // here, the thr desctructor gets called (terminated)
} 