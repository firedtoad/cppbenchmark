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
template <class S> static void BenchEraseOrderSetInt(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < 65536; i++)
    {
        s.insert(i);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

template <class S> static void BenchEraseUnOrderSetInt(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < 65536; i++)
    {
        s.insert(i);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = s.unordered_erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, std::set<int>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, tsl::vector_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::vector_set<int>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, absl::btree_set<int>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, phmap::btree_set<int>);
std::vector<std::string> keys(65536);
template <class S> static void BenchEraseOrderSetString(benchmark::State &state)
{
    S s;

    for (auto i = 0; i < 65536; i++)
    {
        s.insert(keys[i]);
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}
template <class S> static void BenchEraseUnorderSetString(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < 65536; i++)
    {
        s.insert(keys[i]);
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = s.unordered_erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}
BENCHMARK_TEMPLATE(BenchEraseOrderSetString, std::set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetString, tsl::vector_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnorderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnorderSetString, tsl::vector_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetString, absl::btree_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetString, phmap::btree_set<std::string>);

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
