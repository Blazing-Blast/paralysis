#include <iostream>
#include <cinttypes>
#include <random>
#include <chrono>

constexpr uint_fast8_t totalTurns = 231;
constexpr uint64_t maxRuns = 1000000000ULL;
constexpr int threadCount = 1 << 10;
constexpr int blockCount = 1 << 10;

__always_inline int main_unthreaded();
__always_inline int main_threaded();
__global__ void roll_cuda(uint_fast8_t *ret, uint64_t seed);
__device__ static __always_inline uint64_t wyrand(uint64_t &seed);

int main()
{
    uint_fast8_t highest = 0;
    uint_fast8_t *results;

    std::random_device rd;

    std::chrono::time_point<std::chrono::high_resolution_clock> before = std::chrono::high_resolution_clock::now();

    const uint64_t seed = ((uint64_t)rd() << 32) | rd();
    
    cudaMallocManaged(&results, blockCount * threadCount * sizeof(*results));

    roll_cuda<<<blockCount, threadCount>>>(results, seed);

    cudaDeviceSynchronize();

    for (size_t i = 0; i < blockCount * threadCount; i++)
    {
        highest = std::max(highest, results[i]);
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> after = std::chrono::high_resolution_clock::now();
    std::chrono::duration time = after - before;

    std::cout << "My record is: " << (int)highest << ".\nIt took me " << time.count()/1000000 << "ms.\n";
    return 0;
}

__global__ void roll_cuda(uint_fast8_t *results, uint64_t seed)
{
    uint_fast8_t highest = 0;

    uint_fast8_t current;
    seed += threadIdx.x + blockIdx.x * threadCount;

    const size_t runs = (maxRuns / (threadCount * blockCount))
                      + ((threadIdx.x + blockIdx.x * blockCount) < (maxRuns % (threadCount * blockCount)));
    for (size_t i = 0; i < runs; i++)
    {
        current = __popcll(wyrand(seed) & wyrand(seed));
        current += __popcll(wyrand(seed) & wyrand(seed));
        current += __popcll(wyrand(seed) & wyrand(seed));
        current += __popcll(wyrand(seed) & wyrand(seed) << (256 - (totalTurns % 256)));
        highest = max(highest, current);
    }
    results[threadIdx.x] = highest;
}

__device__ static __always_inline uint64_t wyrand(uint64_t &seed)
{
    seed += 0xa0761d6478bd642full;
    uint64_t A = seed,
             B = seed ^ 0xe7037ed1a0b428dbull;
    __uint128_t r = A;
    r *= B;
    A = (uint64_t)r;
    B = (uint64_t)(r >> 64);
    return A ^ B;
}