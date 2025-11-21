#include <iostream>
#include <string>
#include <utility>
#include <vector>

// This file walks through the rule of 0/3/5/6 by contrasting three tiny
// types. The goal is to see how the compiler generates special members and
// when you need to spell them out yourself.

// Rule of Zero: all members are value types that already manage themselves.
// The compiler can synthesize everything safely.
struct LogMessage {
    std::string text;              // owns its memory
    std::vector<std::string> tags; // also owns its memory
    // No explicit destructor/ctor/assignment needed.
};

// Rule of Three: if you declare a destructor, you likely need copy/move
// control as well (pre-C++11 wording). Here we manage a raw pointer to show
// why manual definitions matter.
struct Buffer3 {
    explicit Buffer3(std::size_t size)
        : size(size), data(new int[size]{})
    {
    }

    ~Buffer3() { delete[] data; }

    Buffer3(const Buffer3& other)
        : size(other.size), data(new int[other.size])
    {
        std::copy(other.data, other.data + size, data);
        std::cout << "Buffer3 copy ctor\n";
    }

    Buffer3& operator=(const Buffer3& other)
    {
        if (this != &other) {
            Buffer3 temp(other);
            swap(temp);
            std::cout << "Buffer3 copy assignment\n";
        }
        return *this;
    }

    void swap(Buffer3& other) noexcept
    {
        std::swap(size, other.size);
        std::swap(data, other.data);
    }

    std::size_t size{};
    int* data{};
};

// Rule of Five/Six: when moves enter the picture, add move ctor + move
// assignment (and optionally default the rest). Here we reuse Buffer3 but let
// std::unique_ptr shoulder the resource. We still spell out the copy
// operations so the type is copyable *and* movable.
#include <algorithm>
#include <memory>
struct Buffer5 {
    explicit Buffer5(std::size_t size)
        : size(size), data(std::make_unique<int[]>(size))
    {
    }

    ~Buffer5() = default;

    Buffer5(const Buffer5& other)
        : size(other.size), data(std::make_unique<int[]>(other.size))
    {
        std::copy(other.data.get(), other.data.get() + other.size, data.get());
        std::cout << "Buffer5 copy ctor\n";
    }

    Buffer5& operator=(const Buffer5& other)
    {
        if (this != &other) {
            Buffer5 temp(other);
            swap(temp);
            std::cout << "Buffer5 copy assignment\n";
        }
        return *this;
    }

    Buffer5(Buffer5&&) noexcept = default;
    Buffer5& operator=(Buffer5&&) noexcept = default;

    void swap(Buffer5& other) noexcept
    {
        std::swap(size, other.size);
        data.swap(other.data);
    }

    std::size_t size{};
    std::unique_ptr<int[]> data{};
};

int main()
{
    std::cout << "=== Rule of Zero ===\n";
    LogMessage log{"startup", {"info", "bootstrap"}};
    LogMessage log_copy = log; // compiler-provided copy is fine
    std::cout << "log_copy tags: " << log_copy.tags.size() << " entries\n\n";

    std::cout << "=== Rule of Three ===\n";
    Buffer3 buf3(4);
    buf3.data[0] = 7;
    Buffer3 buf3_copy = buf3; // calls custom copy ctor
    std::cout << "buf3_copy[0] = " << buf3_copy.data[0] << "\n\n";

    std::cout << "=== Rule of Five/Six ===\n";
    Buffer5 buf5(8);
    Buffer5 buf5_copy = buf5;   // defaulted copy
    Buffer5 buf5_moved = std::move(buf5); // defaulted move
    std::cout << "buf5_copy size = " << buf5_copy.size << "\n";
    std::cout << "buf5_moved size = " << buf5_moved.size << "\n";
}

