// Copyright 2020 The Division Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Author dietoad@gmail.com && firedtoad@gmail.com

#include <sys/resource.h>

#include "llvm/ADT/SmallVector.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>

static void BM_rowSmall(benchmark::State &state)
{

    const int row = 1024, col = 1024;
    llvm::SmallVector<llvm::SmallVector<char, col>, row> cache;
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
    llvm::SmallVector<llvm::SmallVector<char, col>, row> cache;
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

BENCHMARK(BM_rowSmall);
BENCHMARK(BM_colSmall);

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
    const rlim_t kStackSize = 16 * 1024 * 1024; // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result      = setrlimit(RLIMIT_STACK, &rl);
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