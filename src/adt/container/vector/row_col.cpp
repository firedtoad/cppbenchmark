//
// Created by Administrator on 2022/01/15.
//
#include <sys/resource.h>

#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include "llvm/ADT/SmallVector.h"

static void BM_rowSmall(benchmark::State &state)
{

    const int row = 1024, col = 1024;
    llvm::SmallVector<llvm::SmallVector<uint64_t,col>,row> cache;
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

static void BM_colSmall(benchmark::State &state)
{
    const int row = 1024, col = 1024;
    llvm::SmallVector<llvm::SmallVector<uint64_t,col>,row> cache;
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
                sum += cache[j][i];
            }
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK(BM_rowSmall);
BENCHMARK(BM_colSmall);

static void BM_row(benchmark::State &state)
{

    const int row = 1024, col = 1024;
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
    const int row = 1024, col = 1024;
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
    const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}