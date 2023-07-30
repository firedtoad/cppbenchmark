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
#include "absl/container/node_hash_map.h"
#include "parallel_hashmap/btree.h"
#include "sparsepp/spp.h"
#include "tsl/ordered_map.h"
#include "tsl/ordered_set.h"
#include "tsl/sparse_map.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <map>
#include <algorithm/container.h>
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
std::vector<uint64_t> ikeys(8192);
template <class M> static void BenchOrderMapIntStd(benchmark::State &state)
{
    M m;

    for (auto i = 0; i < 8192; i++)
    {
        m[ikeys[i]] = i;
    }
    for (auto _ : state)
    {
        int idx = ikeys[_random() % 8192];
        auto it = std::upper_bound(m.begin(), m.end(), idx, [](auto &val, auto &&it) { return val < it.first; });
        benchmark::DoNotOptimize(it);
    }
}

template <class M> static void BenchOrderMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 8192; i++)
    {
        m[ikeys[i]] = i;
    }
    for (auto _ : state)
    {
        int idx = ikeys[_random() % 8192];
        auto it = m.upper_bound(idx);
        benchmark::DoNotOptimize(it);
    }
    std::map<int, int> x;
}

BENCHMARK_TEMPLATE(BenchOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, absl::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, phmap::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapIntStd, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapIntStd, tsl::vector_map<int, int>);
// BENCHMARK_TEMPLATE(BenchOrderMapInt, boost::container::small_flat_map<int, int, 1024>);
std::vector<std::string> keys(8192);
template <class M> static void BenchOrderMapStringStd(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 8192; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 8192;
        auto it     = std::upper_bound(m.begin(), m.end(), keys[kIndex], [](auto &val, auto &&it) { return val < it.first; });
        benchmark::DoNotOptimize(it);
    }
}

template <class M> static void BenchOrderMapString(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 8192; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 8192;
        auto it     = m.upper_bound(keys[kIndex]);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapStringStd, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapStringStd, tsl::vector_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, absl::btree_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, phmap::btree_map<std::string, int>);

int main(int argc, char **argv)
{
    for (auto i = 0; i < 8192; i++)
    {
        keys[i]  = "12345678901234561234567890123456" + std::to_string(_random());
        ikeys[i] = _random() % 8192;
    }

    std::vector<std::string> vs={
        "1234567890",
        "1234567891",
        "1234567892",
        "1234567893",
        "1234567894",
        "1234567895",
        "1234567896",
        "1234567897",
        "1234567898",
        "1234567899",
    };
    std::set<std::string> ss;
    std::deque<std::string> ds;
    absl::c_move(vs,std::back_inserter(ds));
    absl::c_move(ds,std::inserter(ss,ss.end()));
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
