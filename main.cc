#include <iostream>
#include <cinttypes>
#include <thread>
#include <bit>
#include "wy.hh"

constexpr uint_fast8_t totalTurns = 231;
constexpr uint_fast8_t neededTurns = 177;
constexpr int maxRuns = 1000000000;
const uint_fast8_t threadCount = std::thread::hardware_concurrency();

void roll(const int maxRuns, uint_fast8_t *ret);

int main()
{
    uint_fast8_t highest;
    std::thread *threads = new std::thread[threadCount];
    uint_fast8_t *highScores = new uint_fast8_t[threadCount];

    for (char i = 0; i < threadCount; i++)
    {
        threads[i] = std::thread(roll, maxRuns / threadCount, &highScores[i]);
    }
    roll(maxRuns % threadCount, &highest);
    for (char i = 0; i < threadCount; i++)
    {
        threads[i].join();
        highest = std::max(highest, highScores[i]);
    }

    std::cout << "My record is: " << (int)highest << std::endl;
    return 0;
}

void roll(int maxRuns, uint_fast8_t *ret)
{
    wy::rand wyRand;

    std::uniform_int_distribution<uint_fast64_t> distribution(0, UINT_FAST64_MAX);
    uint_fast8_t highest = 0;
    uint_fast8_t current = 0;
    uint_fast64_t x;

    for (size_t _i = 0; _i < maxRuns; _i++)
    {
        for (uint_fast8_t _j = 0; _j < totalTurns / 64; _j++)
        {
            x = wyRand() & wyRand();
            current += std::popcount(x);
        }

        x = wyRand() & wyRand();
        x &= (1ULL << (totalTurns % 64)) - 1;
        current += std::popcount(x);

        highest = std::max(highest, current);
        current = 0;
    }
    *ret = highest;
}
