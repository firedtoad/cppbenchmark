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