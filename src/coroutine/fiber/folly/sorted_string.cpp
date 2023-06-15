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

#include "container/SortedVector.h"
#include "parallel_hashmap/btree.h"
#include "parallel_hashmap/phmap.h"
#include "tsl/ordered_map.h"
#include "tsl/ordered_set.h"
#include "tsl/sparse_map.h"
#include "tsl/sparse_set.h"
#include "utils/memory.h"
#include "utils/rss.h"
#include <EASTL/vector_map.h>
#include <benchmark/benchmark.h>
#include <boost/container/flat_map.hpp>
#include <folly/FBString.h>
#include <folly/container/F14Map.h>
#include <folly/container/heap_vector_types.h>
#include <folly/sorted_vector_types.h>
#include <iostream>
#include <map>
#include <unordered_map>

void *operator new[](size_t size, const char * /*pName*/, int /*flags*/, unsigned /*debugFlags*/, const char * /*file*/, int /*line*/)
{
    return operator new[](size);
}

void *operator new[](size_t size, size_t /*alignment*/, size_t /*alignmentOffset*/, const char * /*pName*/, int /*flags*/, unsigned /*debugFlags*/,
                     const char * /*file*/, int /*line*/)
{
    return operator new[](size);
}
static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 103456789, y = 362436069, z = 521088629;
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

struct Pod
{
    int i;
    Pod() noexcept = default;
    Pod(uint64_t i_) : i(i_) {}
};
std::vector<std::string> keys(65536);

void initKeys(int n)
{
    keys.resize(n);
    for (auto i = 0; i < n; i++)
    {
        keys[i] = "12345678901234561234567890123456" + std::to_string(_random());
    }
}

template <typename V> static void BenchInsert(benchmark::State &state)
{
    initKeys(state.range(0));
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v[keys[i]] = i;
        }
    }
}

// BENCHMARK_TEMPLATE(BenchInsert, sorted_vector_map<std::string, uint32_t>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchInsert, folly::sorted_vector_map<std::string, uint32_t>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchInsert, folly::heap_vector_map<std::string, uint32_t>)->Range(1, 1 << 16);

BENCHMARK_TEMPLATE(BenchInsert, std::map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, phmap::btree_map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, std::unordered_map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, phmap::node_hash_map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, phmap::flat_hash_map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, tsl::sparse_map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, tsl::ordered_map<std::string, uint32_t>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsert, tsl::vector_map<std::string, uint32_t>)->Range(1, 1 << 16);

// BENCHMARK_TEMPLATE(BenchInsert, boost::container::flat_map<std::string, uint32_t>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchInsert, eastl::vector_map<std::string, uint32_t>)->Range(1, 1 << 16);

BENCHMARK_TEMPLATE(BenchInsert, folly::F14FastMap<std::string, uint32_t>)->Range(1, 1 << 16);

template <typename V> static void BenchFind(benchmark::State &state)
{
    V m;
    initKeys(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % state.range(0);
        auto it     = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(it);
    }
}

// BENCHMARK_TEMPLATE(BenchFind, sorted_vector_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchFind, folly::sorted_vector_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchFind, folly::heap_vector_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, std::map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, phmap::btree_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, std::unordered_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, phmap::flat_hash_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, phmap::node_hash_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, tsl::sparse_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, tsl::ordered_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchFind, boost::container::flat_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchFind, eastl::vector_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchFind, folly::F14FastMap<std::string, Pod>)->Range(1, 1 << 16);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V m;
    initKeys(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        m[keys[i]] = i;
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            sum += it.second.i;
        }
    }
    benchmark::DoNotOptimize(sum);

    phmap::flat_hash_map<std::string, uint32_t> pm;
}

// BENCHMARK_TEMPLATE(BenchRange, sorted_vector_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchRange, folly::sorted_vector_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchRange, folly::heap_vector_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, std::map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, phmap::btree_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, std::unordered_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, phmap::node_hash_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, phmap::flat_hash_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, tsl::sparse_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, tsl::ordered_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchRange, boost::container::flat_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchRange, eastl::vector_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchRange, folly::F14FastMap<std::string, Pod>)->Range(1, 1 << 16);

template <typename V> static void BenchErase(benchmark::State &state)
{
    V m;
    initKeys(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % state.range(0);
        auto it     = m.erase(keys[kIndex]);
        if (it > 0)
        {
            m[keys[kIndex]] = kIndex;
        }
    }
}

// BENCHMARK_TEMPLATE(BenchErase, sorted_vector_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchErase, folly::sorted_vector_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchErase, folly::heap_vector_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, std::map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, phmap::btree_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, std::unordered_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, phmap::node_hash_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, phmap::flat_hash_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, tsl::sparse_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, tsl::ordered_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchErase, boost::container::flat_map<std::string, Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchErase, eastl::vector_map<std::string, Pod>)->Range(1, 1 << 16);
BENCHMARK_TEMPLATE(BenchErase, folly::F14FastMap<std::string, Pod>)->Range(1, 1 << 16);

template <typename V> static void BenchStringFind(benchmark::State &state)
{
    V s = "";
    for (auto i = 0; i < state.range(); i++)
    {
        s += std::to_string(_random() % UINT64_MAX);
    }
    auto needle = std::to_string(_random() % UINT32_MAX);

    for (auto _ : state)
    {
        auto it = s.find(needle);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchStringFind, std::string)->Range(128, 1 << 16);
BENCHMARK_TEMPLATE(BenchStringFind, folly::fbstring)->Range(128, 1 << 16);

int main(int argc, char **argv)
{
    //    std::cout << std::is_trivially_copyable_v<std::pair<std::string, Pod>> << '\n';
    //    BM_MemoryStringMap<sorted_vector_map<std::string, uint64_t>, 1 << 20>();
    //    BM_MemoryStringMap<folly::sorted_vector_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<std::map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMapNoSSO<std::map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMap<phmap::btree_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMapNoSSO<phmap::btree_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMap<std::unordered_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<std::unordered_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<phmap::node_hash_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<phmap::node_hash_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<phmap::flat_hash_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<phmap::flat_hash_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<tsl::sparse_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMapNoSSO<tsl::sparse_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMap<tsl::sparse_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<tsl::sparse_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<tsl::sparse_pg_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<tsl::sparse_pg_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<tsl::ordered_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMapNoSSO<tsl::ordered_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMap<tsl::vector_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMapNoSSO<tsl::vector_map<std::string, uint64_t>, 1 << 20, false>();
    RSSMemoryStringMap<tsl::ordered_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<tsl::ordered_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<tsl::vector_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<tsl::vector_map<std::string, uint64_t>, 1 << 20>();
    ////    RSSMemoryStringMap<boost::container::flat_map<std::string, uint64_t>, 1 << 20>();
    ////    RSSMemoryStringMap<eastl::vector_map<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<folly::F14FastMap<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<folly::F14FastMap<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMap<folly::F14ValueMap<std::string, uint64_t>, 1 << 20>();
    RSSMemoryStringMapNoSSO<folly::F14ValueMap<std::string, uint64_t>, 1 << 20>();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}