#include "trace.hpp"

#include <iostream>
#include <utility>

Trace make_trace(const char* label) {
    return Trace{label}; // prvalue
}

Trace&& take_and_return(Trace&& t) {
    std::cout << "forwarding function returns an xvalue" << '\n';
    return std::move(t);
}

void inspect(Trace& t) {
    std::cout << "inspect(Trace&) -> lvalue" << '\n';
}

void inspect(const Trace& t) {
    std::cout << "inspect(const Trace&) -> const lvalue" << '\n';
}

void inspect(Trace&& t) {
    std::cout << "inspect(Trace&&) -> xvalue" << '\n';
}

int main() {
    Trace named{"named"};
    inspect(named);                 // lvalue
    inspect(std::move(named));      // xvalue because std::move turns it into an xvalue

    inspect(make_trace("temporary")); // prvalue binds to rvalue overload

    Trace&& xref = take_and_return(make_trace("forwarded"));
    inspect(std::move(xref)); // xref itself is an lvalue, std::move makes it xvalue

    const Trace const_named{"const"};
    inspect(const_named); // binds to const lvalue overload
}
