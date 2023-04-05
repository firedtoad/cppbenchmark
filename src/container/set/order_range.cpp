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

#include "absl/container/btree_set.h"
#include "parallel_hashmap/btree.h"
#include "tsl/ordered_set.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <set>
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

template <class M> static void BenchRangeOrderSetInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(i);
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, std::set<int>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, absl::btree_set<int>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, phmap::btree_set<int>);
std::vector<std::string> keys(65536);
template <class M> static void BenchRangeOrderSetString(benchmark::State &state)
{
    M m;

    for (auto i = 0; i < 65536; i++)
    {
        m.insert(keys[i]);
    }
    std::string r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r = it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderSetString, std::set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetString, absl::btree_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetString, phmap::btree_set<std::string>);

int main(int argc, char **argv)
{
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = "12345678901234561234567890123456" + std::to_string(_random());
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
