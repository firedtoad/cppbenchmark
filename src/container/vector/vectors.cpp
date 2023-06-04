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

#include "container/vector.hpp"
#include <benchmark/benchmark.h>
#include <numeric>
#include <vector>

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(i);
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, std::vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchInsert, llvm::SmallVector<int, 1024>)->Range(2, 1024);

template <typename V> static void BenchReserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(i);
        }
    }
}

BENCHMARK_TEMPLATE(BenchReserve, std::vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchReserve, llvm::SmallVector<int, 1024>)->Range(2, 1024);

template <typename V> static void BenchResize(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        v.resize(state.range(0));
    }
}

BENCHMARK_TEMPLATE(BenchResize, std::vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchResize, llvm::SmallVector<int, 1024>)->Range(2, 1024);

template <typename V> static void BenchAccumulate(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(i);
    }
    for (auto _ : state)
    {
        auto r = std::accumulate(v.begin(), v.end(), 0);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAccumulate, std::vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchAccumulate, llvm::SmallVector<int, 1024>)->Range(2, 1024);

template <typename V> static void BenchAccumulateReverse(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(i);
    }
    for (auto _ : state)
    {
        auto r = std::accumulate(v.rbegin(), v.rend(), 0);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchAccumulateReverse, std::vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchAccumulateReverse, llvm::SmallVector<int, 1024>)->Range(2, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}