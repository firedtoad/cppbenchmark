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