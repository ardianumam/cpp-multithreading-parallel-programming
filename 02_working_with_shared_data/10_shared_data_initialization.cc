#include <iostream>
#include <thread>
#include <vector>

class Singleton {
  public:
	// The copy and move operators are deleted
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	
	Singleton() { std::cout << "Initializing Singleton\n";}
};

Singleton& get_Singleton()
{
    // Initialized by the first thread that executes this code
	static Singleton single;
	return single;
}


void task()
{
	Singleton& single = get_Singleton();
	std::cout << &single << std::endl;
}

int main()
{
	std::vector<std::thread> threads;
	
	for (int i = 0; i < 10; ++i)
		threads.push_back(std::thread(task));
	
	for (auto& thr : threads)
		thr.join();
}