// What happens to an std::thread object
// when an exception is thrown?
#include <thread>
#include <iostream>

// Callable object - thread entry point
void hello()
{
	std::cout << "Hello, Thread!\n";
}

int main()
{
	try {
		// Create an std::thread object
		std::thread thr(hello);

		// Code that might throw an exception
		throw std::exception(); // after exception happens in the parent thread, all objects, including thr will be terminated
		
		// we are late to call join, so, the parent will terminate this first instead of waiting the tread gets finished
		thr.join(); 
	}
	catch (std::exception& e) {
		std::cout << "Exception caught: " << e.what() << '\n';
	}
}