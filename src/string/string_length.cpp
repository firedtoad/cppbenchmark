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
#include <cstring>
#include <iostream>
#include <numeric>

std::string tag = "1234567890123456";

template <typename Ch> inline size_t StrLen(const Ch *s)
{
    const Ch *p = s;
    while (*p)
        ++p;
    return size_t(p - s);
}

template <typename Ch> inline size_t WrapStrLen(const Ch *s)
{
    return strlen(s);
}

static void BenchNaiveStrlen(benchmark::State &state)
{
    tag.resize(state.range(0));
    std::iota(tag.begin(), tag.end(), 1);
    for (auto _ : state)
    {
        auto l=StrLen(tag.c_str());
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BenchNaiveStrlen)->Range(1, 8192);
;

static void BenchSysStrlen(benchmark::State &state)
{
    tag.resize(state.range(0));
    std::iota(tag.begin(), tag.end(), 1);
    for (auto _ : state)
    {
        auto l=strlen(tag.c_str());
        benchmark::DoNotOptimize(l);
    }
}

BENCHMARK(BenchSysStrlen)->Range(1, 8192);

static void BenchWrapStrLen(benchmark::State &state)
{
    tag.resize(state.range(0));
    std::iota(tag.begin(), tag.end(), 1);
    for (auto _ : state)
    {
        auto l=WrapStrLen(tag.c_str());
        benchmark::DoNotOptimize(l);
    }
}

BENCHMARK(BenchWrapStrLen)->Range(1, 8192);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}