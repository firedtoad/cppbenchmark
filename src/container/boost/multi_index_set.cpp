// Copyright 2023 The Division Authors.
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
#include <benchmark/benchmark.h>
#include <boost/container/flat_set.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>

#include <EASTL/vector_set.h>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/scope_exit.hpp>
#include <set>
#include <tsl/ordered_set.h>
#include <tsl/sparse_set.h>
#include <utils/rss.h>

void *operator new[](size_t size, const char * /*pName*/, int /*flags*/, unsigned /*debugFlags*/, const char * /*file*/, int /*line*/)
{
    return operator new[](size);
}

void *operator new[](size_t size, size_t /*alignment*/, size_t /*alignmentOffset*/, const char * /*pName*/, int /*flags*/, unsigned /*debugFlags*/,
                     const char * /*file*/, int /*line*/)
{
    return operator new[](size);
}

template <typename V>
using MultiContainer =
    boost::multi_index::multi_index_container<V,
                                              boost::multi_index::indexed_by<boost::multi_index::ordered_unique<boost::multi_index::identity<V>>,
                                                                             boost::multi_index::sequenced<>, boost::multi_index::random_access<>>>;

template <typename V>
using MultiHashContainer =
    boost::multi_index::multi_index_container<V, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::identity<V>>>>;

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
    uint64_t i;
    Pod() noexcept = default;
    Pod(uint64_t i_) : i(i_) {}
    bool operator<(const Pod &p) const
    {
        return i < p.i;
    }
};

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v.insert(r);
        }
    }
}

template <typename V> static void BenchInsertMulti(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, sorted_vector<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::ordered_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::sparse_set<uint32_t>)->Range(1, 1 << 10);

BENCHMARK_TEMPLATE(BenchInsert, boost::container::flat_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsertMulti, MultiContainer<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsertMulti, MultiHashContainer<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, eastl::vector_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto it = v.find(_random() % 65536);
        benchmark::DoNotOptimize(it);
    }
}

template <typename V, size_t N> static void BenchFindMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.template get<N>().find(r);
        benchmark::DoNotOptimize(it);
    }
}

template <typename V> static void BenchFindMultiHash(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.find(r);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchFind, sorted_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::ordered_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::sparse_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, boost::container::flat_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindMulti, MultiContainer<uint64_t>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindMultiHash, MultiHashContainer<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, eastl::vector_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            sum += it;
        }
    }
    benchmark::DoNotOptimize(sum);
}

template <typename V, size_t N> static void BenchRangeMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    auto sum = 0;

    for (auto _ : state)
    {
        auto &indices = v.template get<N>();
        for (auto it = indices.begin(); it != indices.end(); it++)
        {
            sum += *it;
        }
    }
    benchmark::DoNotOptimize(sum);
}

template <typename V> static void BenchRangeSet(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    auto sum = 0;

    for (auto _ : state)
    {
        for (auto &&it : v)
        {
            sum += it;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::ordered_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::sparse_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, boost::container::flat_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<uint32_t>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<uint32_t>, 1)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<uint32_t>, 2)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiHashContainer<uint32_t>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeSet, eastl::vector_set<uint32_t>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state)
{

    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.erase(r);
        if (it > 0)
        {
            v.insert(r);
        }
    }
}

template <typename V> static void BenchEraseMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.erase(r);
        if (it > 0)
        {
            v.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchErase, sorted_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::ordered_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::sparse_set<uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, boost::container::flat_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, eastl::vector_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchEraseMulti, MultiContainer<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchEraseMulti, eastl::vector_set<uint32_t>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    {

        rusage rUsage{};
        {
            FillRSS(rUsage);
            MultiContainer<uint32_t> v;
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                v.insert(_random());
            }
            PrintUsage(rUsage);
        }
        {
            FillRSS(rUsage);
            MultiContainer<std::string> v;
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                v.insert(std::to_string(i));
            }
            PrintUsage(rUsage);
        }
        {
            FillRSS(rUsage);
            sorted_vector<uint32_t> sv;
            sv.reserve(1024 * 1024);
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                sv.insert(i);
            }
            PrintUsage(rUsage);
        }
        {
            FillRSS(rUsage);
            tsl::ordered_set<uint32_t> spp;
            spp.reserve(1024 * 1024);
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                spp.insert(_random());
            }
            PrintUsage(rUsage);
        }
        {
            FillRSS(rUsage);
            tsl::sparse_set<uint32_t> spp;
            spp.reserve(1024 * 1024);
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                spp.insert(_random());
            }
            PrintUsage(rUsage);
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}