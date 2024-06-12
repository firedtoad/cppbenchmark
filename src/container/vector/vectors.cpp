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

#include "algorithm/container.h"
#include "container/vector.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
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

    std::vector<int> vi = {10, 9, 8, 7, 5, 4, 3, 2, 1};

    auto it = c_upper_bound(vi, 10, [](auto value, auto it) { return value > it; });
    auto rit=std::make_reverse_iterator(it);
    while(rit!=vi.rend())
    {
        std::cout << *rit << '\n';
        rit++;
    }
    {
//        1:100;10000:120;50000:140;100000:160;200000:180;300000:200;400000:220;500000:240;555000:260;610000:280;1000000:300
    std::vector<std::pair<int,int>> vecScore={
            {1,100},
            {10000,120},
            {50000,140},
            {100000,160},
            {200000,180},
            {300000,200},
            {400000,220},
            {500000,240},
            {555000,260},
            {610000,280},
            {1000000,300},
        };
    auto it = c_lower_bound(vecScore, 200001, []( const std::pair<int, int>& it, int nValue)
        {
            return it.first < nValue;
        });
    auto dist=std::distance(vecScore.begin(),it);
    auto nScore=vecScore[dist].second;
    std::cout<<nScore<<'\n';

    }


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}