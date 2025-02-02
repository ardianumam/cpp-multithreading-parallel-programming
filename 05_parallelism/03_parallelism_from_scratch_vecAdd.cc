#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <numeric>

// Function for single-threaded vector addition
void vector_add_single(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result) {
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
}

// Function for multi-threaded vector addition
void vector_add_parallel(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

int main() {
    const size_t vector_size = 10000000; // Large vector size to see the time difference
    const int num_threads = std::thread::hardware_concurrency(); // Get the number of available cores
    std::cout << "Number of thread (system capability = n cores): " << num_threads << std::endl;

    std::vector<int> a(vector_size, 1); // Initialize vectors with values
    std::vector<int> b(vector_size, 2);
    std::vector<int> result(vector_size);

    double total_time_single = 0.0;
    double total_time_parallel = 0.0;
    const int num_runs = 50;

    for (int run = 0; run < num_runs; ++run) {
        // Measure time for single-threaded vector addition
        auto start_time = std::chrono::high_resolution_clock::now();
        vector_add_single(a, b, result);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
        total_time_single += elapsed.count();

        // Measure time for multi-threaded vector addition
        start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        int chunk_size = vector_size / num_threads;
        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end = (i == num_threads - 1) ? vector_size : (i + 1) * chunk_size;
            threads.emplace_back(vector_add_parallel, std::ref(a), std::ref(b), std::ref(result), start, end);
        }
        for (auto& t : threads) {
            t.join();
        }
        end_time = std::chrono::high_resolution_clock::now();
        elapsed = end_time - start_time;
        total_time_parallel += elapsed.count();
    }

    double avg_time_single = total_time_single / num_runs;
    double avg_time_parallel = total_time_parallel / num_runs;

    std::cout << "Average time for single-threaded vector addition: " << avg_time_single << " ms" << std::endl;
    std::cout << "Average time for multi-threaded vector addition: " << avg_time_parallel << " ms" << std::endl;

    return 0;
}
