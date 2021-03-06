#include <benchmark/benchmark.h>
#include <functional>
#include <map>
#include <unordered_map>

using Handler = int (*)();
static std::map<int, Handler> map_handler;
static std::unordered_map<int, Handler> unmap_handler;

// static std::array<Handler, 65536> array_handler;
static Handler array_handler[65536];

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long _random()
{
    return xorshf96();
}

static void BM_map(benchmark::State &state)
{
    for (auto i = 0; i < 65536; i++)
    {
        map_handler[i] = []() -> int { return 1; };
    }

    for (auto _ : state)
    {
        auto idx      = _random() % 65536;
        const auto it = map_handler.find(idx);
        if (it != map_handler.end())
        {
            benchmark::DoNotOptimize(it->second());
        }
    }
}

static void BM_unmap(benchmark::State &state)
{
    unmap_handler.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        unmap_handler[i] = []() -> int { return 1; };
    }

    for (auto _ : state)
    {
        auto idx      = _random() % 65536;
        const auto it = unmap_handler.find(idx);
        if (it != unmap_handler.end())
        {
            benchmark::DoNotOptimize(it->second());
        }
    }
}

static void BM_array(benchmark::State &state)
{
    for (auto i = 0; i < 65536; i++)
    {
        array_handler[i] = []() -> int { return 1; };
    }
    for (auto _ : state)
    {
        auto idx = _random() % 65536;
        if (array_handler[idx])
        {
            benchmark::DoNotOptimize(array_handler[idx]());
        }
    }
}

BENCHMARK(BM_map);
BENCHMARK(BM_unmap);
BENCHMARK(BM_array);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}