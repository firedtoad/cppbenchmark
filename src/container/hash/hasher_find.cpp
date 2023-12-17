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
#include "internal/city.h"
#include "internal/murmurhash3.h"
#include "internal/wyhash.h"
#include "internal/xxhash.h"
#include "flat_hash_map/bytell_hash_map.hpp"
#include "flat_hash_map/flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood/robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_map.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/htrie_map.h"
#include "tsl/robin_map.h"
#include "tsl/sparse_map.h"
#include "flat_hash_map/unordered_map.hpp"
#include <absl/container/flat_hash_map.h>
#include <benchmark/benchmark.h>
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

struct Hasher
{
    size_t operator()(const std::string &str) const
    {
        return wyhash(str.c_str(), str.size(), 0, _wyp);
    }
};

struct HasherStr
{
    size_t operator()(const char* str,size_t size) const
    {
        return wyhash(str, size, 0, _wyp);
    }
};
std::vector<std::string> keys(65536);
template <class M> static void BenchUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
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

template <class M> static void BenchUnOrderMapStringLoadFactor(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    m.max_load_factor(1.0);
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


template <class M> static void BenchCharKeyMap(benchmark::State &state)
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

BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapStringLoadFactor, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapStringLoadFactor, std::unordered_map<std::string, int,Hasher>);

BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::unordered_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::bytell_hash_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, phmap::flat_hash_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, absl::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, absl::flat_hash_map<std::string, int, absl::Hash<std::string>>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, absl::flat_hash_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, robin_hood::unordered_flat_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, spp::sparse_hash_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::bhopscotch_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::hopscotch_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::sparse_map<std::string, int,Hasher>);
BENCHMARK_TEMPLATE(BenchCharKeyMap, tsl::htrie_map<char, int>);
BENCHMARK_TEMPLATE(BenchCharKeyMap, tsl::htrie_map<char, int,HasherStr>);
BENCHMARK_TEMPLATE(BenchCharKeyMap, tsl::array_map<char, int>);
BENCHMARK_TEMPLATE(BenchCharKeyMap, tsl::array_map<char, int,HasherStr>);

int main(int argc, char **argv)
{

    for (auto i = 0; i < 65536; i++)
    {
        keys[i]    = "12345678901234561234567890123456" + std::to_string(_random());
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
