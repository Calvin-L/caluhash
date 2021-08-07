// This is the example from the README, verbatim.

#include <caluhash.hpp>
#include <random>
#include <iostream>

struct MyType {
    int field1;
    char field2;
};

// Teach the library how to hash a new type.
caluhash::Hasher& operator<<(caluhash::Hasher& h, const MyType& x) {
    return h << x.field1 << x.field2;
}

int main() {

    // Source of randomness.  Pick your favorite from <random>!
    std::mt19937_64 generator;

    // A randomly-selected hash function.  It will output numbers in the range
    // [0, 2^10).  Pass 64 instead of 10 to get the maximum possible range.
    //
    // The range parameter is useful if your number of buckets is a power of 2;
    // you will not have to "re-hash" the output to fit in how many buckets you
    // have.
    caluhash::HashFunction h(generator, 10);

    MyType x;
    std::cout << "h(x) = " << h(x) << std::endl;

    return 0;
}
