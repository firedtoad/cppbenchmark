//
// Created by Administrator on 2022/01/15.
//

#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>

static void BM_row(benchmark::State &state)
{
    const int row = 10240, col = 1024;
    std::vector<std::vector<uint64_t>> cache;
    cache.resize(row);
    for (auto &it : cache)
    {
        it.resize(col);
    }
    uint64_t sum{};
    for (auto _ : state)
    {
        for (auto i = 0; i < row; i++)
        {
            for (auto j = 0; j < col; j++)
            {
                sum += cache[i][j];
            }
        }
    }
    benchmark::DoNotOptimize(sum);
}

static void BM_col(benchmark::State &state)
{
    const int row = 10240, col = 1024;
    std::vector<std::vector<uint64_t>> cache;
    cache.resize(row);
    for (auto &it : cache)
    {
        it.resize(col);
    }
    uint64_t sum{};
    for (auto _ : state)
    {
        for (auto i = 0; i < col; i++)
        {
            for (auto j = 0; j < row; j++)
            {
                sum += cache[j][i];
            }
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK(BM_row);
BENCHMARK(BM_col);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}