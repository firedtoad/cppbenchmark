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
#include "parallel_hashmap/btree.h"
#include "parallel_hashmap/phmap.h"
#include "robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include "tsl/ordered_set.h"
#include "tsl/robin_set.h"
#include "tsl/sparse_set.h"
#include "unordered_map.hpp"
#include "utils/symbol.h"
#include <absl/container/flat_hash_set.h>
#include <benchmark/benchmark.h>
#include <set>
#include <unordered_set>
#include <utils/rss.h>

template <class M> static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
        }
    }
}

template <class M> static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.reserve(65536);
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
        }
    }
}

BENCHMARK_TEMPLATE(BM_insert, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BM_insert, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BM_insert, tsl::sparse_set<int>);
BENCHMARK_TEMPLATE(BM_reserve, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BM_reserve, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BM_reserve, tsl::sparse_set<int>);
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>

template <typename T>
using MapList =
    boost::multi_index_container<T, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::identity<T>, std::hash<T>>,
                                                                   boost::multi_index::sequenced<>>>;


struct Pod{
    int x,y,z,w;
};

int main(int argc, char **argv)
{

    {
        std::vector<std::shared_ptr<int>> vec;
        vec.resize(1 << 20);
        rusage rusage;
        FillRSS(rusage);
        for(auto &it:vec)
        {
            it=std::make_shared<int>();
        }
        PrintUsage(rusage);
    }

    {
        std::vector<std::unique_ptr<int>> vec;
        vec.resize(1 << 20);
        rusage rusage;
        FillRSS(rusage);
        for(auto &it:vec)
        {
            it=std::make_unique<int>();
        }
        PrintUsage(rusage);
    }

    BM_MemorySet<std::unordered_set<int>, 1 << 20>();
    BM_MemorySet<ska::unordered_set<int>, 1 << 20>();
    BM_MemorySet<ska::flat_hash_set<int>, 1 << 20>();
    BM_MemorySet<ska::bytell_hash_set<int>, 1 << 20>();
    BM_MemorySet<phmap::flat_hash_set<int>, 1 << 20>();
    BM_MemorySet<absl::flat_hash_set<int>, 1 << 20>();
    BM_MemorySet<robin_hood::unordered_flat_set<int>, 1 << 20>();
    BM_MemorySet<spp::sparse_hash_set<int>, 1 << 20>();
    BM_MemorySet<tsl::bhopscotch_set<int>, 1 << 20>();
    BM_MemorySet<tsl::hopscotch_set<int>, 1 << 20>();
    BM_MemorySet<tsl::robin_set<int>, 1 << 20>();
    BM_MemorySet<tsl::sparse_set<int>, 1 << 20>();
    BM_MemorySet<tsl::ordered_set<int>, 1 << 20>();
    BM_MemorySet<std::set<int>, 1 << 20>();
    BM_MemorySet<MapList<int>, 1 << 20>();
    BM_MemorySet<phmap::btree_set<int>, 1 << 20>();

    BM_MemoryStringSet<std::set<std::string>, 1 << 20>();
    BM_MemoryStringSet<MapList<std::string>, 1 << 20>();
    BM_MemoryStringSet<phmap::btree_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<std::unordered_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<ska::unordered_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<ska::flat_hash_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<ska::bytell_hash_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<phmap::flat_hash_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<absl::flat_hash_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<robin_hood::unordered_flat_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<spp::sparse_hash_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<tsl::bhopscotch_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<tsl::hopscotch_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<tsl::robin_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<tsl::sparse_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<tsl::sparse_pg_set<std::string>, 1 << 20>();
    BM_MemoryStringSet<tsl::ordered_set<std::string>, 1 << 20>();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}