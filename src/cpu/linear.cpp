//
// Created by Administrator on 2022/08/10.
//

#include <benchmark/benchmark.h>
#include <iostream>

#include <vector>

struct S
{
    uint64_t s;
    uint64_t c[3];
};

static void BM_IntervalPtr(benchmark::State &state)
{
    int row = state.range(0);
    std::vector<S *> cache;
    cache.reserve(row);
    for (auto i = 0; i < row; i++)
    {
        cache.emplace_back(new S);
    }
    uint64_t sum{};
    for (auto _ : state)
    {
        for (auto &s : cache)
        {
            sum += s->s;
        }
    }
    for (auto &s : cache)
    {
        delete s;
    }
    benchmark::DoNotOptimize(sum);
}

static void BM_LinearPtr(benchmark::State &state)
{
    int row = state.range(0);
    std::vector<S *> cache;
    cache.reserve(row);
    S *ss = new S[row];
    for (auto i = 0; i < row; i++)
    {
        cache.emplace_back(&ss[i]);
    }
    uint64_t sum{};
    for (auto _ : state)
    {
        for (auto &s : cache)
        {
            sum += s->s;
        }
    }
    delete[] ss;
    benchmark::DoNotOptimize(sum);
}

static void BM_LinearVal(benchmark::State &state)
{
    int row = state.range(0);
    std::vector<S> cache;
    cache.reserve(row);
    for (auto i = 0; i < row; i++)
    {
        cache.emplace_back();
    }

    uint64_t sum{};
    for (auto _ : state)
    {
        for (auto &s : cache)
        {
            sum += s.s;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK(BM_IntervalPtr)->Range(1, 1 << 13);
BENCHMARK(BM_LinearPtr)->Range(1, 1 << 13);
BENCHMARK(BM_LinearVal)->Range(1, 1 << 13);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}