#include "../include/matrix.hpp"

#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

using day3::Matrix;
using namespace std::chrono;

Matrix make_matrix(std::size_t n, double start_value)
{
    Matrix m(n, n);
    double value = start_value;
    for (std::size_t r = 0; r < n; ++r) {
        for (std::size_t c = 0; c < n; ++c) {
            m.at(r, c) = value;
            value += 1.0;
        }
    }
    return m;
}

duration<double, std::milli> benchmark_copy(const Matrix& source, int iterations)
{
    auto begin = high_resolution_clock::now();
    volatile double checksum = 0.0; // avoid optimizing away the copies
    for (int i = 0; i < iterations; ++i) {
        Matrix copy = source; // copy constructor
        checksum += copy.at(0, 0);
    }
    (void)checksum;
    return high_resolution_clock::now() - begin;
}

duration<double, std::milli> benchmark_move(std::vector<Matrix>& pool)
{
    auto begin = high_resolution_clock::now();
    volatile double checksum = 0.0;
    for (Matrix& candidate : pool) {
        Matrix moved = std::move(candidate); // move constructor
        checksum += moved.at(0, 0);
    }
    (void)checksum;
    return high_resolution_clock::now() - begin;
}

int main()
{
    const std::size_t dimension = 128;
    const int iterations = 300;

    std::cout << "Building baseline matrix..." << std::flush;
    Matrix baseline = make_matrix(dimension, 1.0);
    std::cout << "done\n";

    std::cout << "Precomputing move pool..." << std::flush;
    std::vector<Matrix> pool;
    pool.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        pool.emplace_back(make_matrix(dimension, static_cast<double>(i)));
    }
    std::cout << "done\n\n";

    auto copy_time = benchmark_copy(baseline, iterations);
    auto move_time = benchmark_move(pool);

    std::cout << "Copy time: " << copy_time.count() << " ms (" << iterations << " copies)\n";
    std::cout << "Move time: " << move_time.count() << " ms (" << iterations << " moves)\n";

    if (move_time < copy_time) {
        std::cout << "Moves are faster (as expected).\n";
    } else {
        std::cout << "Copy elision or optimizer may hide the benefit—try -O0 to see clearer gaps.\n";
    }
}

