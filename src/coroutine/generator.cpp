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
#include <cppcoro/generator.hpp>
#include <iostream>
cppcoro::generator<const std::uint64_t> acc(int c)
{
    uint64_t r = 0;
    while (true)
    {
        co_yield r;

        r = 0;
        for (auto i = 0; i < c; i++)
        {
            r += i;
        }
    }
}

static void BenchAccumulate(benchmark::State &state)
{
    int r = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            r += i;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAccumulate)->Range(1024, 1024);

static void BenchAccumulateGen(benchmark::State &state)
{
    int r  = 0;
    auto f = acc(state.range(0));
    auto g = f.begin();
    for (auto _ : state)
    {
        ++g;
        r += *g;
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAccumulateGen)->Range(1024, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
