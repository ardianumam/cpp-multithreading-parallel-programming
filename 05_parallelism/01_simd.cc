#include <immintrin.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

// Function to add arrays using SIMD
void addArraysSIMD(const float* a, const float* b, float* result, int size) {
    for (int i = 0; i < size; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);      // Load 8 floats from array a
        __m256 vb = _mm256_loadu_ps(&b[i]);      // Load 8 floats from array b
        __m256 vr = _mm256_add_ps(va, vb);       // Add the floats
        _mm256_storeu_ps(&result[i], vr);        // Store the result
    }
}

// Function to add arrays without SIMD
void addArraysNormal(const float* a, const float* b, float* result, int size) {
    for (int i = 0; i < size; ++i) {
        result[i] = a[i] + b[i];                 // Add the floats
    }
}

int main() {
    const int size = 1000000;                    // Large array size for noticeable difference
    std::vector<float> a(size, 1.0f);
    std::vector<float> b(size, 2.0f);
    std::vector<float> result(size);

    const int iterations = 50;
    std::vector<double> simd_times;
    std::vector<double> normal_times;

    // Measure SIMD performance
    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        addArraysSIMD(a.data(), b.data(), result.data(), size);
        auto end = std::chrono::high_resolution_clock::now();
        simd_times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
    }

    // Measure normal performance
    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        addArraysNormal(a.data(), b.data(), result.data(), size);
        auto end = std::chrono::high_resolution_clock::now();
        normal_times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
    }

    // Calculate average times
    double avg_simd_time = std::accumulate(simd_times.begin(), simd_times.end(), 0.0) / iterations;
    double avg_normal_time = std::accumulate(normal_times.begin(), normal_times.end(), 0.0) / iterations;

    std::cout << "Average SIMD Time: " << avg_simd_time << " ms\n";
    std::cout << "Average Normal Time: " << avg_normal_time << " ms\n";

    return 0;
}
