//
// Created by Administrator on 2022/01/16.
//

#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>

struct alignas(8) cache_unaligned
{
    uint64_t n;
};

struct alignas(64) cache_aligned
{
    uint64_t n;
};

template <typename T> static void BenchShare(benchmark::State &state)
{
    static std::vector<T> data;
    data.resize(state.threads());
    auto idx = state.thread_index();
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(data[idx].n++);
        }
    }
    state.SetItemsProcessed(100000);
}

template <typename T> static void BenchNoShare(benchmark::State &state)
{
    std::vector<T> data;
    data.resize(state.threads());
    auto idx = state.thread_index();
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(data[idx].n++);
        }
    }
    state.SetItemsProcessed(100000);
}

BENCHMARK_TEMPLATE(BenchShare, cache_unaligned)->ThreadRange(1, 16)->Range(100000, 100000);
BENCHMARK_TEMPLATE(BenchShare, cache_aligned)->ThreadRange(1, 16)->Range(100000, 100000);

BENCHMARK_TEMPLATE(BenchNoShare, cache_unaligned)->ThreadRange(1, 16)->Range(100000, 100000);
BENCHMARK_TEMPLATE(BenchNoShare, cache_aligned)->ThreadRange(1, 16)->Range(100000, 100000);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}