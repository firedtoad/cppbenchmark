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