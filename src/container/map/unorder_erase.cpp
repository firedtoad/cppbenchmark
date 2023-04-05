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

#include "bytell_hash_map.hpp"
#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_map.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/htrie_map.h"
#include "tsl/robin_map.h"
#include "tsl/sparse_map.h"
#include "unordered_map.hpp"
#include <absl/container/flat_hash_map.h>
#include <benchmark/benchmark.h>
#include <iostream>
#include <unordered_map>

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
std::vector<std::string> keys(65536);

template <class M> static void BenchEraseUnOrderMapInt(benchmark::State &state)
{
    M m;
    m.reserve(65536);
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

BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, ska::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, ska::bytell_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, absl::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, robin_hood::unordered_flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, spp::sparse_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::bhopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::hopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::robin_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::sparse_map<int, int>);

template <class M> static void BenchEraseUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]]   = i;
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

template <class M> static void BenchEraseCharKeyMap(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]]   = i;
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

BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, absl::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseCharKeyMap, tsl::htrie_map<char, int>);
BENCHMARK_TEMPLATE(BenchEraseCharKeyMap, tsl::array_map<char, int>);

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
