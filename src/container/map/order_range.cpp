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

#include "absl/container/btree_map.h"
#include "parallel_hashmap/btree.h"
#include "tsl/ordered_map.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <map>
static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long _random()
{
    return xorshf96();
}
template <class M> static void BenchRangeOrderMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

template <class M> static void BenchRangeOrderVectorInt(benchmark::State &state)
{
    M m;
    m.resize(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = {i, i};
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderVectorInt, std::vector<std::pair<int, int>>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, absl::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, phmap::btree_map<int, int>);

template <class M> static void BenchRangeOrderMapString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]    = "12345678901234561234567890123456" + std::to_string(_random());
        m[keys[i]] = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, absl::btree_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, phmap::btree_map<std::string, int>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
