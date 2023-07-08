// Copyright 2023 The Division Authors.
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

#include "algorithm/container.h"
#include "parallel_hashmap/btree.h"
#include "tsl/ordered_set.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <set>
/* A George Marsaglia generator, period 2^96-1 */
static unsigned long x = 123456789, y = 362436069, z = 521288629;
static unsigned long xorshf96()
{
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

static void _random_init()
{
    x = 123456789, y = 362436069, z = 521288629;
}

static inline unsigned long _random()
{
    return xorshf96();
}
template <class S> static void BenchSetEraseNodesIf(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < state.range(0); i++)
    {
        s.insert(i);
    }
    _random_init();
    for (auto _ : state)
    {
        int r   = _random() % state.range(0);
        auto sz = c_erase_nodes_if(s, [r](auto &&it) { return it == r; });
        if (sz > 0)
        {
            s.insert(r);
        }
    }
}

template <class S, bool LinearBased = false> static void BenchSetEraseElementsIf(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < state.range(0); i++)
    {
        s.insert(i);
    }
    _random_init();
    for (auto _ : state)
    {
        int r   = _random() % state.range(0);
        auto sz = c_erase_elements_if<LinearBased>(s, [r](auto &&it) { return it == r; });
        if (sz > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchSetEraseNodesIf, std::set<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetEraseElementsIf, std::set<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetEraseElementsIf, phmap::btree_set<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetEraseElementsIf, tsl::ordered_set<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetEraseElementsIf, tsl::vector_set<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetEraseElementsIf, tsl::ordered_set<int>, true)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetEraseElementsIf, tsl::vector_set<int>, true)->Range(1, 65536);

std::vector<std::string> keys(65536);
template <class S> static void BenchSetStringEraseNodesIf(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < state.range(0); i++)
    {
        s.insert(keys[i]);
    }
    _random_init();
    for (auto _ : state)
    {
        auto r  = keys[_random() % state.range(0)];
        auto sz = c_erase_nodes_if(s, [r](auto &&it) { return it == r; });
        if (sz > 0)
        {
            s.insert(r);
        }
    }
}
template <class S, bool LinearBased = false> static void BenchSetStringEraseElementsIf(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < state.range(0); i++)
    {
        s.insert(keys[i]);
    }
    _random_init();
    for (auto _ : state)
    {
        auto r  = keys[_random() % state.range(0)];
        auto sz = c_erase_elements_if<LinearBased>(s, [r](auto &&it) { return it == r; });
        if (sz > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchSetStringEraseNodesIf, std::set<std::string>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetStringEraseElementsIf, std::set<std::string>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetStringEraseElementsIf, phmap::btree_set<std::string>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetStringEraseElementsIf, tsl::ordered_set<std::string>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetStringEraseElementsIf, tsl::vector_set<std::string>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetStringEraseElementsIf, tsl::ordered_set<std::string>, true)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchSetStringEraseElementsIf, tsl::vector_set<std::string>, true)->Range(1, 65536);

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
