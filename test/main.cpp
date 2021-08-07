#include <caluhash.hpp>
#include <cassert>
#include <random>
#include <iostream>

// ---------------------------------------------------------------------------
// A demonstration of how to implement hashing for a custom type.

struct CustomType {
  bool a;
  int b;
};

caluhash::Hasher& operator<<(caluhash::Hasher& h, const CustomType& x) {
  return h << x.a << x.b;
}

// ---------------------------------------------------------------------------
// A few very trivial tests.

int main() {

  std::mt19937_64 generator;

  int collisionCount = 0;
  for (int i = 0; i < 100; ++i) {
    std::cout << "iteration " << i << std::endl;

    caluhash::HashFunction h(generator, 5);

    std::cout << "  h(0) = " << h(0) << std::endl;
    std::cout << "  h(1) = " << h(1) << std::endl;
    std::cout << "  h(2) = " << h(2) << std::endl;
    std::cout << "  h(3) = " << h(3) << std::endl;
    std::cout << "  h(4) = " << h(4) << std::endl;
    std::cout << "  h(5) = " << h(5) << std::endl;

    // technically this can fail with small probability
    if (h(0) == h(1)) {
      ++collisionCount;
    }

    for (int j = 0; j < 100; ++j) {
      assert(h(j) < 32);
    }

    CustomType x { true, 100 };
    std::cout << "custom hash = " << h(x) << std::endl;

  }

  assert(collisionCount <= 3);

  for (int i = 0; i < 100; ++i) {
    caluhash::HashFunction h1(generator, 64);
    caluhash::HashFunction h2(generator, 64);

    // technically this can fail with samll probability
    assert(h1(100) != h2(100));
  }

}
