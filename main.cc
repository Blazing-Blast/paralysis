#include <iostream>
#include <cinttypes>
#include <cmath>
#include <random>

int main()
{
    uint_fast8_t highest = 0;
    uint_fast8_t current = 0;
    uint_fast32_t x;
    constexpr int maxRuns = 1000000000;
    constexpr uint_fast8_t totalTurns = 231;
    constexpr uint_fast8_t neededTurns = 177;
    constexpr int dims = 4;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint_fast32_t> distribution(0, UINT_FAST32_MAX);

    for (size_t _i = 0; _i < maxRuns; _i++)
    {
        for (uint_fast8_t _j = 0; _j < (totalTurns >> dims); _j++)
        {
            x = distribution(gen);
            for (uint_fast8_t _k = 0; _k < (1 << dims); _k++)
            {
                current += ((x & 3) == 0);
                x >>= 2;
            }
        }
        x = distribution(gen);
        for (uint_fast8_t _k = 0; _k < (totalTurns % (1 << dims)); _k++)
        {
            current += ((x & 3) == 0);
            x >>= 2;
        }
        highest = std::max(highest, current);
        current = 0;
    }
    std::cout << "My record is: " << (int)highest << std::endl;
    return 0;
}
