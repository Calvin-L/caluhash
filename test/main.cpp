#include <caluhash.hpp>
#include <cassert>
#include <random>
#include <iostream>
#include <string>
#include <set>

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

void testBasics() {

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

    // technically this can fail with small probability
    assert(h1(100) != h2(100));
  }

}

// ---------------------------------------------------------------------------
// Test string hashing.

// NOTE: This definition has to be in `namespace std` so that
// "argument-dependent lookup" (ADL) can find it.  See:
// https://clang.llvm.org/compatibility.html#dep_lookup (retrieved 2021/8/16)
//
// In general, for types defined in namespaces (like std::string), the
// operator<< override has to be in the same namespace as the type.  It could
// also be in the caluhash namespace---although I personally find that uglier,
// since it isn't obvious why some definitions have to be in ::caluhash and
// others don't---but it's up to you.
namespace std {
caluhash::Hasher& operator<<(caluhash::Hasher& h, const std::string& x) {
  h << x.size();
  for (char c : x) {
    h << c;
  }
  return h;
}
}

void testStrings() {
  std::mt19937_64 generator;

  // warm up...
  for (int i = 0; i < 10000; ++i) {
    std::uniform_int_distribution<uint64_t>()(generator);
  }

  // For a bunch of random hash functions, verify that the strings
  //   "axxxxx...x"
  //   "bxxxxx...x"
  //   "cxxxxx...x"
  // etc.  all have different hashes.  This ensures that differences EARLY in
  // a hashable type still have an effect on the overall hash result.
  for (int i = 0; i < 1000; ++i) {
    caluhash::HashFunction h(generator, 64);
    std::set<uint64_t> hashes;

    std::string s;
    for (int j = 0; j < 10000; ++j) {
      s.push_back('x');
    }

    for (char c = 'a'; c <= 'z'; ++c) {
      s[0] = c;
      hashes.insert(h(s));
    }

    std::cout << "Got " << hashes.size() << " distinct hashes" << std::endl;
    assert(hashes.size() == 26);
  }
}

// ---------------------------------------------------------------------------
// All tests.

int main() {
  testBasics();
  testStrings();
}
