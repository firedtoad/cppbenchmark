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
#include "tsl/ordered_map.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <map>

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
std::vector<uint64_t> ikeys;
template <class M> static void BenchOrderMapInt(benchmark::State &state)
{
    M m;

    for (auto i = 0; i < 65536; i++)
    {
        m[ikeys[i]] = i;
    }
    for (auto _ : state)
    {
        auto idx = ikeys[_random() % 65536];
        auto c   = m.find(idx);
        auto v   = c->second;
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, absl::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, phmap::btree_map<int, int>);
BENCHMARK_TEMPLATE(
    BenchOrderMapInt,
    tsl::ordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<int, int>>, std::vector<std::pair<int, int>>>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, absl::btree_map<int, int>);
// BENCHMARK_TEMPLATE(BenchOrderMapInt, boost::container::small_flat_map<int, int, 65536>);
std::vector<std::string> keys(65536);
template <class M> static void BenchOrderMapString(benchmark::State &state)
{
    M m;

    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, tsl::ordered_map<std::string, int, std::hash<std::string>, std::equal_to<std::string>,
                                                         std::allocator<std::pair<std::string, int>>, std::vector<std::pair<std::string, int>>>);
BENCHMARK_TEMPLATE(BenchOrderMapString, absl::btree_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, phmap::btree_map<std::string, int>);
// BENCHMARK_TEMPLATE(BenchOrderMapString, boost::container::small_flat_map<std::string, int, 1024>);

template <class M> static void BenchCombineMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[ikeys[i]] = i;
    }
    for (auto _ : state)
    {
        auto idx = ikeys[_random() % 65536];
        auto c   = m.find(idx);
        auto v   = c->second;
        benchmark::DoNotOptimize(v);
    }
}

template <class M> static void BenchTwiceMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        auto r     = ikeys[i];
        uint32_t f = r >> 32;
        uint32_t s = r & 0xFFFFFFFF;
        m[f][s]    = i;
    }
    for (auto _ : state)
    {
        auto idx   = ikeys[_random() % 65536];
        uint32_t f = idx >> 32;
        uint32_t s = idx & 0xFFFFFFFF;

        auto c = m.find(f);
        if (c != m.end())
        {
            auto d = c->second.find(s);
            auto v = d->second;
            benchmark::DoNotOptimize(v);
        }
    }
}

BENCHMARK_TEMPLATE(BenchCombineMapInt, std::map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, std::map<uint32_t, std::map<uint32_t, uint32_t>>);

BENCHMARK_TEMPLATE(BenchCombineMapInt, std::unordered_map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>>);

int main(int argc, char **argv)
{
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]  = "12345678901234561234567890123456" + std::to_string(_random());
        ikeys[i] = _random();
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
