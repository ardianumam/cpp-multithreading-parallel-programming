#include <future>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

using namespace std::literals;

int task()
{
    std::cout << "Executing task() in thread with ID: ";
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(5s);
    std::cout << "Returning from task()\n";
    return 42;
}

int main()
{
    std::cout << "In main thread with ID: " << std::this_thread::get_id() << '\n';

    // Async option
    std::cout << "Calling async with option \"async\"\n";
    std::future<int> result_async = std::async(std::launch::async, task);
    std::cout << "After the async call\n";
    std::cout << "Async get() result: " << result_async.get() << '\n';
    std::cout << "After the async get()\n";

    return 0;
}