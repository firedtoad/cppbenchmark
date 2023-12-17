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
#include "flat_hash_map/bytell_hash_map.hpp"
#include "flat_hash_map/flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood/robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_set.h"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include "tsl/htrie_set.h"
#include "tsl/ordered_set.h"
#include "tsl/robin_set.h"
#include "tsl/sparse_set.h"
#include "flat_hash_map/unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <iostream>
#include <unordered_set>

template <typename T>
using MapList =
    boost::multi_index_container<T, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::identity<T>, std::hash<T>>,
                                                                   boost::multi_index::sequenced<>>>;

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

template <class M> static void BenchRangeUnOrderSetInt(benchmark::State &state)
{
    M m;
    m.reserve(65536);
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

BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, ska::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, ska::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, ska::bytell_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, phmap::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, absl::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::vector_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::sparse_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, MapList<int>);

std::vector<std::string> keys(65536);
template <class M> static void BenchRangeUnOrderSetString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
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

template <class M> static void BenchRangeUnOrderSetStringMulti(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(keys[i]);
    }
    std::string r{};
    for (auto _ : state)
    {
        for (auto &it : m.template get<1>())
        {
            r = it;
        }
    }
    benchmark::DoNotOptimize(r);
}

template <class M> static void BenchRangeCharKeySet(benchmark::State &state)
{
    M m;

    for (auto i = 0; i < 65536; i++)
    {
        m.insert(keys[i]);
    }
    std::string r{};
    for (auto _ : state)
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            r = it.key();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, std::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, ska::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, ska::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, ska::bytell_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, phmap::node_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, phmap::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, absl::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::vector_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::sparse_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, MapList<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetStringMulti, MapList<std::string>);
BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::array_set<char>);

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
