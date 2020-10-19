#include <chrono>
#include <cmath>
#include <algorithm>

#include "java_random.hh"

JavaRandom::JavaRandom() {
  using namespace std::chrono;
  uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  setSeed(ms);
  _haveNextNextGaussian = false;
}

JavaRandom::JavaRandom(int64_t seed) {
  setSeed(seed);
  _haveNextNextGaussian = false;
}

void JavaRandom::setSeed(int64_t seed) {
  _seed = (seed ^ 0x5deece66d) & ((1L << 48) - 1);
}

int32_t JavaRandom::next(int bits) {
  if (bits < 1)
    bits = 1;
  if (bits > 32)
    bits = 32;

  this->_seed = (_seed * 0x5deece66dL + 0xbL) & ((1L << 48) - 1);

  return (_seed >> (48 - bits));
}

void JavaRandom::nextBytes(uint8_t *bytes, size_t length) {
  for (size_t i = 0; i < length; ) {
    for (int32_t rnd = nextInt(), n = std::min(static_cast<int>(length - i), static_cast<int>(4)); n-- > 0; rnd >>= 8) {
      bytes[i++] = rnd;
    }
  }
}

int32_t JavaRandom::nextInt() {
  return next(32);
}

int32_t JavaRandom::nextInt(int32_t n) {
  if ((n & ~n) == n)
    return (n * next(31)) >> 31;
  int32_t bits, val;
  do {
    bits = next(31);
    val = bits % n;
  } while (bits - val + (n - 1) < 0);
  return val;
}

int64_t JavaRandom::nextLong() {
  return (static_cast<int64_t>(next(32)) << 32) + next(32);
}

bool JavaRandom::nextBoolean() {
  return next(1) != 0;
}

float JavaRandom::nextFloat() {
  return next(24) / static_cast<float>(1 << 24);
}

double JavaRandom::nextDouble() {
  return ((static_cast<int64_t>(next(26)) << 27) + next(27)) / static_cast<double>(1L << 53);
}

double JavaRandom::nextGaussian() {
  if (_haveNextNextGaussian) {
    _haveNextNextGaussian = false;
    return _nextNextGaussian;
  } else {
    double v1, v2, s;
    do {
      v1 = 2 * nextDouble() - 1;
      v2 = 2 * nextDouble() - 1;
      s = v1 * v1 + v2 * v2;
    } while (s >= 1 || s == 0);
    double multiplier = sqrt(-2 * log(s) / s);
    _nextNextGaussian = v2 * multiplier;
    _haveNextNextGaussian = true;
    return v1 * multiplier;
  }
}
