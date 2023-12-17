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

#include "algorithm/container.h"
#include "flat_hash_map/bytell_hash_map.hpp"
#include "flat_hash_map/flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood/robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_map.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/htrie_map.h"
#include "tsl/ordered_map.h"
#include "tsl/robin_map.h"
#include "tsl/sparse_map.h"
#include "flat_hash_map/unordered_map.hpp"
#include <absl/container/flat_hash_map.h>
#include <benchmark/benchmark.h>
#include <iostream>
#include <unordered_map>

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
std::vector<std::string> keys(65536);
template <class M> static void BenchMapEraseNodesIf(benchmark::State &state)
{

    M m;
    for (auto i = 0; i < state.range(0); i++)
    {
        m[i] = i;
    }
    _random_init();
    for (auto _ : state)
    {
        int r   = _random() % state.range(0);
        auto sz = c_erase_nodes_if(m, [r](auto &&it) { return it.first == r; });
        if (sz > 0)
        {
            m[r] = r;
        }
    }
}

template <class M, bool LinearBased = false> static void BenchMapEraseElementsIf(benchmark::State &state)
{
    M m;
    m.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        m[i] = i;
    }
    _random_init();
    for (auto _ : state)
    {
        int r   = _random() % state.range(0);
        auto sz = c_erase_elements_if<LinearBased>(m, [r](auto &&it) { return it.first == r; });
        if (sz > 0)
        {
            m[r] = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchMapEraseNodesIf, std::unordered_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, std::unordered_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, ska::unordered_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, ska::flat_hash_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, ska::bytell_hash_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, phmap::flat_hash_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, phmap::node_hash_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, robin_hood::unordered_flat_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, spp::sparse_hash_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::bhopscotch_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::hopscotch_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::robin_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::sparse_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::ordered_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::vector_map<int, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::ordered_map<int, int>, true)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapEraseElementsIf, tsl::vector_map<int, int>, true)->Range(1, 65536);

template <class M> static void BenchMapStringEraseNodesIf(benchmark::State &state)
{
    M m;
    m.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        m[keys[i]] = i;
    }
    _random_init();
    for (auto _ : state)
    {
        auto r  = keys[_random() % state.range(0)];
        auto sz = c_erase_nodes_if(m, [r](auto &&it) { return it.first == r; });
        if (sz > 0)
        {
            m[r] = 0;
        }
        benchmark::DoNotOptimize(r);
    }
}

template <class M, bool LinearBased = false> static void BenchMapStringEraseElementsIf(benchmark::State &state)
{
    M m;
    m.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        m[keys[i]] = i;
    }
    _random_init();
    for (auto _ : state)
    {
        auto r  = keys[_random() % state.range(0)];
        auto sz = c_erase_elements_if<LinearBased>(m, [r](auto &&it) { return it.first == r; });
        if (sz > 0)
        {
            m[r] = 0;
        }
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchMapStringEraseNodesIf, std::unordered_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, std::unordered_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, ska::unordered_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, ska::flat_hash_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, ska::bytell_hash_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, phmap::flat_hash_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, robin_hood::unordered_flat_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, spp::sparse_hash_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::bhopscotch_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::hopscotch_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::robin_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::sparse_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::ordered_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::vector_map<std::string, int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::ordered_map<std::string, int>, true)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchMapStringEraseElementsIf, tsl::vector_map<std::string, int>, true)->Range(1, 65536);

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
