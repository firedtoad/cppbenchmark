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

#include "SortedVector.h"
#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>
#include <benchmark/benchmark.h>
#include <boost/container/flat_map.hpp>
#include <folly/FBString.h>
#include <folly/container/heap_vector_types.h>
#include <folly/container/F14Map.h>
#include <folly/sorted_vector_types.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include "utils/rss.h"
#include "tsl/sparse_map.h"

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

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v[r]   = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, sorted_vector_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, folly::sorted_vector_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, folly::heap_vector_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::sparse_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, boost::container::flat_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, eastl::vector_map<uint32_t, uint32_t>)->Range(1, 1 << 10);

BENCHMARK_TEMPLATE(BenchInsert, folly::F14FastMap<uint32_t, uint32_t>)->Range(1, 1 << 10);
template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v[r]   = r;
    }
    for (auto _ : state)
    {
        auto it = v.find(_random() % 65536);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchFind, sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, folly::sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, folly::heap_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::sparse_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, boost::container::flat_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, eastl::vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, folly::F14FastMap<uint64_t, Pod>)->Range(1, 1 << 10);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v[r]   = r;
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            sum += it.second.i;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, folly::sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, folly::heap_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::sparse_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, boost::container::flat_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, eastl::vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, folly::F14FastMap<uint64_t, Pod>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v[r]   = r;
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.erase(r);
        if (it > 0)
        {
            v[r] = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchErase, sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, folly::sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, folly::heap_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::sparse_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, boost::container::flat_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, eastl::vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, folly::F14FastMap<uint64_t, Pod>)->Range(1, 1 << 10);

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
        auto it=s.find(needle);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchStringFind, std::string)->Range(128, 1 << 16);
BENCHMARK_TEMPLATE(BenchStringFind, folly::fbstring)->Range(128, 1 << 16);

int main(int argc, char **argv)
{
    std::cout << std::is_trivially_copyable_v<std::pair<uint64_t, Pod>> << '\n';
    BM_MemoryMap<sorted_vector_map<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<folly::sorted_vector_map<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<std::map<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<std::unordered_map<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<boost::container::flat_map<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<eastl::vector_map<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<folly::F14FastMap<uint64_t, uint64_t>, 1 << 20>();
    BM_MemoryMap<folly::F14ValueMap<uint64_t, uint64_t>, 1 << 20>();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}