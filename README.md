# Calvin's (Almost) Universal Hashing Library

This is a dirt-simple C++ library for [universal hashing](https://en.wikipedia.org/wiki/Universal_hashing).
It is useful if you need to churn out many different hash functions for the
same data type.  [Bloom filters](https://en.wikipedia.org/wiki/Bloom_filter),
for example, need many different hash functions.  If you are implementing a
server, you might want to generate a random hash function to prevent [hash
collision DoS attacks](https://en.wikipedia.org/wiki/Collision_attack#Usage_in_DoS_attacks).

Note that this library implements the "almost universal" rolling hash scheme.
It is fast and high-quality in practice, but doesn't offer the strongest
guarantees of a "true" universal hash function generator.

## How to use this library

```c++
#include <caluhash.hpp>
#include <random>
#include <iostream>

struct MyType {
    int field1;
    char field2;
};

// Teach the library how to hash a new type.
caluhash::Hasher& operator<<(caluhash::Hasher& h, const MyType& x) {
    return h << x.field1 << x.field2 << ...;
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
```

See also: `test/main.cpp`.

## How to include this library in your project

If you use CMake:

```cmake
cmake_minimum_required(VERSION 3.14) # or higher

...

include(FetchContent)
FetchContent_Declare(
    caluhash_repo
    GIT_REPOSITORY https://github.com/Calvin-L/caluhash.git
    GIT_TAG        main)
FetchContent_MakeAvailable(caluhash_repo)

...

target_link_libraries(YOUR_TARGET_HERE PRIVATE caluhash)
```

Note that you may want to replace `main` with a specific revision for stable
builds.
