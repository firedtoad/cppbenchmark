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

#include <algorithm>
#include <benchmark/benchmark.h>
#include <random>
#include <vector>

using Fn = int (*)();

int a()
{
    return 1;
}

int b()
{
    return 1;
}

static void BM_seq(benchmark::State &state)
{
    std::vector<Fn> callers(100000);
    auto half = callers.size() / 2;
    size_t i  = 0;
    for (auto &it : callers)
    {
        if (i++ < half)
        {
            it = a;
            continue;
        }
        it = b;
    }
    for (auto _ : state)
    {
        for (auto &it : callers)
        {
            auto r = it();
            benchmark::DoNotOptimize(r);
        }
    }
}

static void BM_alter(benchmark::State &state)
{
    std::vector<Fn> callers(100000);
    auto i = 0;
    for (auto &it : callers)
    {
        if ((i++ % 2) == 0)
        {
            it = a;
            continue;
        }
        it = b;
    }
    for (auto _ : state)
    {
        for (auto &it : callers)
        {
            auto r = it();
            benchmark::DoNotOptimize(r);
        }
    }
}

static void BM_rand(benchmark::State &state)
{
    std::vector<Fn> callers(100000);
    auto i = 0;
    for (auto &it : callers)
    {
        if ((i++ % 2) == 0)
        {
            it = a;
            continue;
        }
        it = b;
    }
    std::random_device rd;
    std::shuffle(callers.begin(), callers.end(), rd);
    for (auto _ : state)
    {
        for (auto &it : callers)
        {
            auto r = it();
            benchmark::DoNotOptimize(r);
        }
    }
}

BENCHMARK(BM_seq);
BENCHMARK(BM_alter);
BENCHMARK(BM_rand);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}