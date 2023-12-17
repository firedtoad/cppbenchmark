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

#include "flat_hash_map/bytell_hash_map.hpp"
#include "flat_hash_map/flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood/robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_set.h"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include "tsl/htrie_set.h"
#include "tsl/robin_set.h"
#include "tsl/sparse_set.h"
#include "flat_hash_map/unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <unordered_set>

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SparseSet.h>
#include <llvm/ADT/StringSet.h>

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
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::sparse_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, llvm::DenseSet<int>);

template <class M> static void BenchRangeVecSetInt(benchmark::State &state)
{
    M m;
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

BENCHMARK_TEMPLATE(BenchRangeVecSetInt, llvm::SetVector<int>);

template <class M> static void BenchRangeSparseSetInt(benchmark::State &state)
{
    M m;
    m.setUniverse(65536);
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

BENCHMARK_TEMPLATE(BenchRangeSparseSetInt, llvm::SparseSet<unsigned>);

template <class M> static void BenchRangeUnOrderSetString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        m.insert(sKey);
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

template <class M> static void BenchRangeCharKeySet(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        m.insert(sKey);
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
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, phmap::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::sparse_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, llvm::DenseSet<llvm::StringRef>);

template <class M> static void BenchRangeStringSet(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(std::to_string(i));
    }
    std::string r{};
    for (auto _ : state)
    {
        for (auto it = m.begin(); it != m.end(); it++)
        {
            r = it->first();
        }
    }
    benchmark::DoNotOptimize(r);
}
BENCHMARK_TEMPLATE(BenchRangeStringSet, llvm::StringSet<>);

BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::array_set<char>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}
