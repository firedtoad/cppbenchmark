#include <benchmark/benchmark.h>
#include <cppcoro/generator.hpp>
#include <iostream>
cppcoro::generator<const std::uint64_t> acc(int c)
{
    uint64_t r = 0;
    while (true)
    {
        co_yield r;

        r = 0;
        for (auto i = 0; i < c; i++)
        {
            r += i;
        }
    }
}

static void BenchAccumulate(benchmark::State &state)
{
    int r = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            r += i;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAccumulate)->Range(1024, 1024);

static void BenchAccumulateGen(benchmark::State &state)
{
    int r  = 0;
    auto f = acc(state.range(0));
    auto g = f.begin();
    for (auto _ : state)
    {
        ++g;
        r += *g;
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAccumulateGen)->Range(1024, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
