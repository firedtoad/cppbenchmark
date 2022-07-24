#include <algorithm>
#include <benchmark/benchmark.h>
#include <vector>

using Fn = int (*)();

int a()
{
    return 1;
}

int b()
{
    return 1;
}

static void BM_seq(benchmark::State &state)
{
    std::vector<Fn> callers(100000);
    auto half = callers.size() / 2;
    size_t i  = 0;
    for (auto &it : callers)
    {
        if (i++ < half)
        {
            it = a;
            continue;
        }
        it = b;
    }
    for (auto _ : state)
    {
        for (auto &it : callers)
        {
            benchmark::DoNotOptimize(it());
        }
    }
}

static void BM_alter(benchmark::State &state)
{
    std::vector<Fn> callers(100000);
    auto i = 0;
    for (auto &it : callers)
    {
        if ((i++ % 2) == 0)
        {
            it = a;
            continue;
        }
        it = b;
    }
    for (auto _ : state)
    {
        for (auto &it : callers)
        {
            benchmark::DoNotOptimize(it());
        }
    }
}

static void BM_rand(benchmark::State &state)
{
    std::vector<Fn> callers(100000);
    auto i = 0;
    for (auto &it : callers)
    {
        if ((i++ % 2) == 0)
        {
            it = a;
            continue;
        }
        it = b;
    }
    std::random_shuffle(callers.begin(), callers.end());
    for (auto _ : state)
    {
        for (auto &it : callers)
        {
            benchmark::DoNotOptimize(it());
        }
    }
}

BENCHMARK(BM_seq);
BENCHMARK(BM_alter);
BENCHMARK(BM_rand);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}