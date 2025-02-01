// Example of passing a std::thread object to a function
#include <thread>
#include <iostream>

// The task function
void hello() {
	using namespace std::literals;
	std::this_thread::sleep_for(1s);
	std::cout << "Hello, Thread!\n";
}

// Function taking a thread object as argument
// The object must be moved into the argument
void func(std::thread thr) {
	std::cout << "Received thread with ID " << thr.get_id() << std::endl;

	// The function argument now "owns" the system thread
	// It is responsible for calling ajoin()
	thr.join();
}

int main() {
	// std::thread is a move-only object
	std::thread thr(hello);

	// Display the child thread's ID
	std::cout << "Hello thread has ID (before): " << thr.get_id() << '\n';

	// Pass a named object using std::move()
	func(std::move(thr));
	std::cout << "Hello thread has ID (after): " << thr.get_id() << '\n';

	// Or ass a temporary object
	// func(std::thread(hello));

}