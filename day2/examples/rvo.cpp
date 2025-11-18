#include "trace.hpp"

#include <iostream>

Trace make_elidable() {
    std::cout << "make_elidable returning a prvalue" << '\n';
    return Trace{"elidable"};
}

Trace make_conditional(bool flag) {
    Trace first{"first"};
    Trace second{"second"};
    std::cout << "make_conditional returning " << (flag ? "first" : "second") << '\n';
    if (flag) {
        return first; // cannot be elided because of conditional path
    }
    return second;
}

int main() {
    std::cout << "-- Guaranteed copy elision (C++17) --" << '\n';
    Trace a = make_elidable();

    std::cout << "\n-- Conditional return (no guaranteed elision) --" << '\n';
    Trace b = make_conditional(false);

    std::cout << "\n-- Build with -O0 vs -O2 and compare --" << '\n';
    std::cout << "Use -fno-elide-constructors to observe all moves/copies" << '\n';

    (void)a;
    (void)b;
}
