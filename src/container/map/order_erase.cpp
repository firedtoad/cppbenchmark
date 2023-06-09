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

template <class M> static void BenchEraseOrderMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, tsl::vector_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, absl::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, phmap::btree_map<int, int>);
std::vector<std::string> keys(65536);
template <class M> static void BenchEraseOrderMapString(benchmark::State &state)
{
    M m;

    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = 0;
        }
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchEraseOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, tsl::vector_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, absl::btree_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, phmap::btree_map<std::string, int>);

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
