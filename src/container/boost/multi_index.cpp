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
#include <benchmark/benchmark.h>
#include <boost/container/flat_map.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>

#include <boost/multi_index/sequenced_index.hpp>
#include <map>
#include <utils/rss.h>

template <typename V>
using MultiContainer = boost::multi_index::multi_index_container<
    V, boost::multi_index::indexed_by<boost::multi_index::ordered_unique<boost::multi_index::identity<V>>, boost::multi_index::sequenced<>>>;

template <typename V>
using MultiHashContainer = boost::multi_index::multi_index_container<
    V, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::member<V, uint32_t, &V::first>>>>;

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
            v[r]   = r;
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
            v.insert(std::pair{r, r});
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, sorted_vector_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, boost::container::flat_map<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsertMulti, MultiContainer<std::pair<uint32_t, uint32_t>>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsertMulti, MultiHashContainer<std::pair<uint32_t, uint32_t>>)->Range(1, 1 << 10);

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

template <typename V> static void BenchFindMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(std::pair{r, r});
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.template get<0>().find(std::pair{r, r});
        benchmark::DoNotOptimize(it);
    }
}

template <typename V> static void BenchFindMultiHash(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(std::pair{r, r});
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.find(r);
        benchmark::DoNotOptimize(it);
    }
    MultiContainer<std::pair<uint32_t, Pod>> vx;
    vx.template get<0>().find(std::pair{1, 1});
}

BENCHMARK_TEMPLATE(BenchFind, sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, boost::container::flat_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindMulti, MultiContainer<std::pair<uint32_t, Pod>>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindMultiHash, MultiHashContainer<std::pair<uint32_t, Pod>>)->Range(1, 1 << 10);

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

template <typename V, size_t N> static void BenchRangeMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(std::pair{r, r});
    }
    auto sum = 0;

    for (auto _ : state)
    {
        auto &indices = v.template get<N>();
        for (auto it = indices.begin(); it != indices.end(); it++)
        {
            sum += it->second.i;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, boost::container::flat_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<std::pair<uint32_t, Pod>>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<std::pair<uint32_t, Pod>>, 1)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiHashContainer<std::pair<uint32_t, Pod>>, 0)->Range(1, 1 << 10);

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

template <typename V> static void BenchEraseMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(std::pair{r, r});
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.erase(std::pair{r, r});
        if (it > 0)
        {
            v.insert(std::pair{r, r});
        }
    }
}

BENCHMARK_TEMPLATE(BenchErase, sorted_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, boost::container::flat_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchEraseMulti, MultiContainer<std::pair<uint32_t, Pod>>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    {
        rusage rUsage{};
        FillRSS(rUsage);
        //        MultiHashContainer<std::pair<uint32_t, Pod>> v;
        MultiContainer<std::pair<uint32_t, Pod>> v;
        for (auto i = 0; i < 1024 * 1024; i++)
        {
            v.emplace(i, i);
        }
        PrintUsage(rUsage);
        FillRSS(rUsage);
        sorted_vector_map<uint64_t, Pod> sv;
        for (auto i = 0; i < 1024 * 1024; i++)
        {
            sv[i] = i;
        }
        PrintUsage(rUsage);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}