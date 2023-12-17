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

#include "butil/containers/flat_map.h"
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
#include <benchmark/benchmark.h>
#include <boost/container/flat_map.hpp>
#include <iostream>
#include <unordered_map>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/IndexedMap.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/StringMap.h>

template <class M> static void BenchRangeUnOrderMapInt(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, ska::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, ska::bytell_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, robin_hood::unordered_flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, spp::sparse_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::bhopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::hopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::robin_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::sparse_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, llvm::DenseMap<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, llvm::MapVector<int, int>);

// static void BenchRangeFlatMapInt(benchmark::State &state)
//{
//     butil::FlatMap<int, int> m;
//     m.init(65536);
//     for (auto i = 0; i < 65536; i++)
//     {
//         m[i] = i;
//     }
//     int r{};
//     for (auto _ : state)
//     {
//         for (auto it : m)
//         {
//             r += it.second;
//         }
//     }
//     benchmark::DoNotOptimize(r);
// }
//
// BENCHMARK(BenchRangeFlatMapInt);

template <class M> static void BenchRangeIndexedMap(benchmark::State &state)
{
    M m;
    m.resize(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (size_t i = 0; i < m.size(); i++)
        {
            r += m[i];
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeIndexedMap, llvm::IndexedMap<int>);

template <class M> static void BenchRangeUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        m[sKey]   = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

template <class M> static void BenchRangeCharKeyMap(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        m[sKey]   = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r = it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, llvm::DenseMap<llvm::StringRef, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, llvm::MapVector<llvm::StringRef, int>);

BENCHMARK_TEMPLATE(BenchRangeCharKeyMap, tsl::htrie_map<char, int>);
BENCHMARK_TEMPLATE(BenchRangeCharKeyMap, tsl::array_map<char, int>);

// static void BenchRangeFlatMapString(benchmark::State &state)
//{
//     butil::FlatMap<std::string, int> m;
//     m.init(65536);
//     for (auto i = 0; i < 65536; i++)
//     {
//         auto sKey = std::to_string(i);
//         m[sKey]   = i;
//     }
//     int r{};
//     for (auto _ : state)
//     {
//         for (auto &it : m)
//         {
//             r += it.second;
//         }
//     }
//     benchmark::DoNotOptimize(r);
// }
//
// BENCHMARK(BenchRangeFlatMapString);

template <class M> static void BenchRangeIndexedMapString(benchmark::State &state)
{
    M m;
    m.resize(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = std::to_string(i);
    }
    int r{};
    for (auto _ : state)
    {
        for (size_t i = 0; i < m.size(); i++)
        {
            r += m[i].size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeIndexedMapString, llvm::IndexedMap<std::string>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
