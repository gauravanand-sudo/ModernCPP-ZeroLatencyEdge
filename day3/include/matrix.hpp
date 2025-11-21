#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>

namespace day3 {

// A simple, dynamically sized matrix that demonstrates the rule of 5/6.
// The class owns its memory and offers copy and move operations with
// explicit, readable implementations.
class Matrix {
public:
    // Rule of Zero would have let the compiler synthesize everything if we
    // only stored a std::vector. Here we purposefully define the special
    // members to make their behavior visible for the tutorial.

    Matrix() = default;

    Matrix(std::size_t rows, std::size_t cols, double value = 0.0)
        : rows_(rows), cols_(cols), values_(rows * cols, value) {}

    Matrix(std::initializer_list<std::initializer_list<double>> rows)
    {
        rows_ = rows.size();
        cols_ = rows.begin()->size();
        values_.reserve(rows_ * cols_);
        for (const auto& r : rows) {
            values_.insert(values_.end(), r.begin(), r.end());
        }
    }

    // Copy constructor (Rule of 3/5/6): deep copies the owned storage.
    Matrix(const Matrix& other)
        : rows_(other.rows_), cols_(other.cols_), values_(other.values_)
    {
        if constexpr (kVerbose) {
            std::cout << "Matrix copy ctor\n";
        }
    }

    // Move constructor (Rule of 5/6): steals the data buffer; source becomes empty.
    Matrix(Matrix&& other) noexcept
        : rows_(other.rows_), cols_(other.cols_), values_(std::move(other.values_))
    {
        other.rows_ = other.cols_ = 0;
        if constexpr (kVerbose) {
            std::cout << "Matrix move ctor\n";
        }
    }

    // Copy assignment (Rule of 3/5/6): strong guarantee via copy-and-swap.
    Matrix& operator=(const Matrix& other)
    {
        if (this != &other) {
            Matrix temp(other);
            swap(temp);
            if constexpr (kVerbose) {
                std::cout << "Matrix copy assignment\n";
            }
        }
        return *this;
    }

    // Move assignment (Rule of 5/6): release current storage, steal from source.
    Matrix& operator=(Matrix&& other) noexcept
    {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            values_ = std::move(other.values_);
            other.rows_ = other.cols_ = 0;
            if constexpr (kVerbose) {
                std::cout << "Matrix move assignment\n";
            }
        }
        return *this;
    }

    ~Matrix() = default;

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    double& at(std::size_t r, std::size_t c) { return values_.at(r * cols_ + c); }
    const double& at(std::size_t r, std::size_t c) const { return values_.at(r * cols_ + c); }

    static Matrix identity(std::size_t n)
    {
        Matrix m(n, n, 0.0);
        for (std::size_t i = 0; i < n; ++i) {
            m.at(i, i) = 1.0;
        }
        return m;
    }

    void fill(double value) { std::fill(values_.begin(), values_.end(), value); }

    void swap(Matrix& other) noexcept
    {
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
        values_.swap(other.values_);
    }

    friend bool operator==(const Matrix& lhs, const Matrix& rhs)
    {
        return lhs.rows_ == rhs.rows_ && lhs.cols_ == rhs.cols_ && lhs.values_ == rhs.values_;
    }

private:
    static constexpr bool kVerbose = false; // flip to true to log special member usage.

    std::size_t rows_{0};
    std::size_t cols_{0};
    std::vector<double> values_{};
};

} // namespace day3

