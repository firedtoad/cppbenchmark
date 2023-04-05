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
#include <map>

static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 521288629;
    uint64_t t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline uint64_t _random()
{
    return xor_shift96();
}
std::vector<std::string> keys(65536);
static void BenchStringLess(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto kIndex  = _random() % 65536;
        auto kIndex1 = _random() % 65536;
        benchmark::DoNotOptimize(keys[kIndex] < keys[kIndex1]);
    }
}

static void BenchStringCompare(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto kIndex  = _random() % 65536;
        auto kIndex1 = _random() % 65536;
        benchmark::DoNotOptimize(keys[kIndex] < keys[kIndex1].c_str());
    }
}

BENCHMARK(BenchStringLess);
BENCHMARK(BenchStringCompare);

template <class M> static void BenchOrderMapStringSSO(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapStringSSO, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapStringSSO, std::map<std::string, int, std::less<>>);
template <class M> static void BenchOrderMapStringNoSSO(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapStringNoSSO, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapStringNoSSO, std::map<std::string, int, std::less<>>);
int main(int argc, char **argv)
{
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]    = "12345678901234561234567890123456" + std::to_string(_random());
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
