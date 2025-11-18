#pragma once

#include <iostream>
#include <string>
#include <utility>

class Trace {
public:
    explicit Trace(std::string label) : label_{std::move(label)}, id_{++next_id_} {
        log("ctor");
    }

    Trace(const Trace& other) : label_{other.label_}, id_{++next_id_} {
        log("copy-ctor", &other);
    }

    Trace(Trace&& other) noexcept : label_{std::move(other.label_)}, id_{++next_id_} {
        log("move-ctor", &other);
    }

    Trace& operator=(const Trace& other) {
        label_ = other.label_;
        log("copy-assign", &other);
        return *this;
    }

    Trace& operator=(Trace&& other) noexcept {
        label_ = std::move(other.label_);
        log("move-assign", &other);
        return *this;
    }

    ~Trace() {
        log("dtor");
    }

    const std::string& label() const noexcept { return label_; }

private:
    std::string label_;
    inline static int next_id_ = 0;
    int id_;

    void log(const char* event, const Trace* other = nullptr) const {
        std::cout << "[Trace #" << id_ << ":" << label_ << "] " << event;
        if (other) {
            std::cout << " <- #" << other->id_ << ":" << other->label_;
        }
        std::cout << '\n';
    }
};
