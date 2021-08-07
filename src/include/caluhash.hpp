#pragma once

#include <cstdint>
#include <random>
#include <cassert>

namespace caluhash {

class Hasher {
public:
  inline Hasher();

  template <class Rng>
  explicit inline Hasher(Rng& sourceOfRandomness);

  inline void push(uint64_t n) noexcept;

  inline uint64_t get(unsigned nbits) const noexcept;

private:
  uint64_t value;
  uint64_t rollingFactor;
  uint64_t finalFactor;
};

inline Hasher& operator<<(Hasher& h,   signed char      i) noexcept;
inline Hasher& operator<<(Hasher& h, unsigned char      i) noexcept;
inline Hasher& operator<<(Hasher& h,   signed short     i) noexcept;
inline Hasher& operator<<(Hasher& h, unsigned short     i) noexcept;
inline Hasher& operator<<(Hasher& h,   signed int       i) noexcept;
inline Hasher& operator<<(Hasher& h, unsigned int       i) noexcept;
inline Hasher& operator<<(Hasher& h,   signed long      i) noexcept;
inline Hasher& operator<<(Hasher& h, unsigned long      i) noexcept;
inline Hasher& operator<<(Hasher& h,   signed long long i) noexcept;
inline Hasher& operator<<(Hasher& h, unsigned long long i) noexcept;

class HashFunction {
public:
  inline HashFunction() = default;

  template <class Rng>
  explicit inline HashFunction(Rng& sourceOfRandomness, unsigned nbits);

  template <class T>
  inline uint64_t operator()(const T& value) const;

private:
  Hasher params;
  unsigned nbits;
};


// ----------------------------------------------------------------------------

Hasher::Hasher()
  : value(0),
    rollingFactor(31),
    finalFactor(1)
{
}

template <class Rng>
Hasher::Hasher(Rng& sourceOfRandomness)
  : value(0),
    rollingFactor(std::uniform_int_distribution<decltype(rollingFactor)>()(sourceOfRandomness)),
    finalFactor(std::uniform_int_distribution<decltype(finalFactor)>()(sourceOfRandomness))
{
}

void Hasher::push(uint64_t n) noexcept {
  value = (value * rollingFactor) + n;
}

uint64_t Hasher::get(unsigned nbits) const noexcept {
  assert(nbits <= 64);
  return nbits ? ((value * finalFactor) >> (64 - nbits)) : 0;
}

inline Hasher& operator<<(Hasher& h,   signed char      i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h, unsigned char      i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h,   signed short     i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h, unsigned short     i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h,   signed int       i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h, unsigned int       i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h,   signed long      i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h, unsigned long      i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h,   signed long long i) noexcept { h.push((uint64_t)i); return h; }
inline Hasher& operator<<(Hasher& h, unsigned long long i) noexcept { h.push((uint64_t)i); return h; }

template <class Rng>
inline HashFunction::HashFunction(Rng& sourceOfRandomness, unsigned _nbits)
  : params(sourceOfRandomness),
    nbits(_nbits)
{
  assert(nbits <= 64);
}

template <class T>
inline uint64_t HashFunction::operator()(const T& value) const {
  Hasher cpy(params);
  cpy << value;
  return cpy.get(nbits);
}

}
