#pragma once
class JavaRandom {
  public:
    JavaRandom();
    JavaRandom(int64_t seed);
    void nextBytes(uint8_t* bytes, size_t length);
    bool nextBoolean();
    double nextDouble();
    float nextFloat();
    double nextGaussian();
    int32_t nextInt();
    int32_t nextInt(int32_t n);
    int64_t nextLong();
    void setSeed(int64_t seed);
    int32_t next(int bits);

  private:
    int64_t _seed;
    double _nextNextGaussian;
    bool _haveNextNextGaussian;
};
