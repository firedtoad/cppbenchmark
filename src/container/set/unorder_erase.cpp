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

#include "absl/container/flat_hash_set.h"
#include "bytell_hash_map.hpp"
#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_set.h"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include "tsl/htrie_set.h"
#include "tsl/robin_set.h"
#include "tsl/sparse_set.h"
#include "tsl/ordered_set.h"
#include "unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <unordered_set>

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

template <class S> static void BenchEraseUnOrderSetInt(benchmark::State &state)
{
    S s;
    s.reserve(65536);
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

template <class S> static void BenchEraseTslUnOrderSetInt(benchmark::State &state)
{
    S s;
    s.reserve(65536);
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


BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, ska::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, ska::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, ska::bytell_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, phmap::node_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, phmap::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, absl::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseTslUnOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseTslUnOrderSetInt, tsl::vector_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::sparse_set<int>);
std::vector<std::string> keys(65536);
template <class S> static void BenchEraseUnOrderSetString(benchmark::State &state)
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

template <class S> static void BenchEraseTslUnOrderSetString(benchmark::State &state)
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

template <class S> static void BenchEraseCharKeySet(benchmark::State &state)
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

BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, std::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, ska::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, ska::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, ska::bytell_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, phmap::node_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, phmap::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, absl::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseTslUnOrderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseTslUnOrderSetString, tsl::vector_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::sparse_set<std::string>);

BENCHMARK_TEMPLATE(BenchEraseCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchEraseCharKeySet, tsl::array_set<char>);

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
