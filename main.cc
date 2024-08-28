#define __AVX2_AVAILABLE__

#include <iostream>
#include <cinttypes>
#include <thread>
#include <bit>
#include <random>
#include "xorshiro256.hh"

typedef SEFUtility::RNG::Xoshiro256Plus<SIMDInstructionSet::AVX2> AVXrand;

constexpr uint_fast8_t totalTurns = 231;
constexpr uint_fast8_t neededTurns = 177;
constexpr int maxRuns = 1000000000;
const uint_fast8_t threadCount = std::thread::hardware_concurrency();

__always_inline int main_unthreaded();
__always_inline int main_threaded();
void roll(const int maxRuns, uint_fast8_t *ret);
void roll256(int maxRuns, uint_fast8_t *ret);

int main()
{
    return main_threaded();
}

__always_inline int main_threaded()
{
    uint_fast8_t highest;
    std::thread *threads = new std::thread[threadCount];
    uint_fast8_t *highScores = new uint_fast8_t[threadCount];

    for (char i = 0; i < threadCount; i++)
    {
        threads[i] = std::thread(roll256, maxRuns / threadCount, &highScores[i]);
    }
    roll256(maxRuns % threadCount, &highest);
    for (char i = 0; i < threadCount; i++)
    {
        threads[i].join();
        highest = std::max(highest, highScores[i]);
    }

    std::cout << "My record is: " << (int)highest << std::endl;
    return 0;
}

__always_inline int main_unthreaded()
{
    uint_fast8_t highest;
    roll256(maxRuns, &highest);
    std::cout << "My record is: " << (int)highest << std::endl;
    return 0;
}

void roll256(int maxRuns, uint_fast8_t *ret)
{
    std::random_device rd;
    const uint32_t seed[2] = {rd(), rd()};
    AVXrand rng((uint64_t)seed);

    uint_fast8_t highest = 0;
    uint_fast8_t current = 0;
    __m256i x;
    __m256i y;
    // __uint128_t lower128;
    __uint128_t r128;

    for (size_t _i = 0; _i < maxRuns; _i++)
    {
        x = rng.next4();
        y = rng.next4();

        x &= y;
        r128 = std::bit_cast<__uint128_t>(_mm256_extracti128_si256(x, 1));
        current = std::__popcount(r128);
        r128 = std::bit_cast<__uint128_t>(_mm256_extracti128_si256(x, 0));
        r128 <<= 256 - totalTurns;
        current += std::__popcount(r128);
        highest = std::max(highest, current);
    }
    *ret = highest;
}