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

#include <EASTL/vector_map.h>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/scope_exit.hpp>
#include <map>
#include <unordered_map>
#include <tsl/ordered_map.h>
#include <tsl/sparse_map.h>
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
using MultiHashContainer = boost::multi_index::multi_index_container<
    V, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::member<V, std::string, &V::first>>>>;

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
std::vector<std::string> keys(65536);
template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v[keys[i]]=i;
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
            auto r = keys[i];
            v.insert(std::pair{r, i});
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, sorted_vector_map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::unordered_map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::sparse_map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::ordered_map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, boost::container::flat_map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, eastl::vector_map<std::string, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsertMulti, MultiContainer<std::pair<std::string, uint32_t>>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsertMulti, MultiHashContainer<std::pair<std::string, uint32_t>>)->Range(1, 1 << 10);


template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v[keys[i]]=i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto it = v.find(keys[kIndex]);
        benchmark::DoNotOptimize(it);
    }
}

template <typename V, size_t N> static void BenchFindMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = keys[i];
        v.insert(std::pair{r, i});
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto it = v.template get<N>().find(std::pair{keys[kIndex], 0});
        benchmark::DoNotOptimize(it);
    }
}

template <typename V> static void BenchFindMultiHash(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = keys[i];
        v.insert(std::pair{r, i});
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto it = v.find(keys[kIndex]);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchFind, sorted_vector_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::unordered_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::sparse_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::ordered_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, eastl::vector_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, boost::container::flat_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindMulti, MultiContainer<std::pair<std::string, Pod>>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindMultiHash, MultiHashContainer<std::pair<std::string, Pod>>)->Range(1, 1 << 10);


template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = keys[i];
        v[r]=i;
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
        auto r = keys[i];
        v.insert(std::pair{r, i});
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

template <typename V> static void BenchRangeSet(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = keys[i];
        v.insert(std::pair{r, i});
    }
    auto sum = 0;

    for (auto _ : state)
    {
        for (auto &&it : v)
        {
            sum += it.second.i;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::unordered_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::sparse_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::ordered_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, boost::container::flat_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<std::pair<std::string, Pod>>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<std::pair<std::string, Pod>>, 1)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiContainer<std::pair<std::string, Pod>>, 2)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRangeMulti, MultiHashContainer<std::pair<std::string, Pod>>, 0)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, eastl::vector_map<std::string, Pod>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state)
{

    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = keys[i];
        v[r]   = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto it = v.erase(keys[kIndex]);
        if (it > 0)
        {
            v[keys[kIndex]] = kIndex;
        }
    }
}

template <typename V> static void BenchEraseMulti(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = keys[i];
        v.insert(std::pair{r, i});
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto it = v.erase(std::pair{keys[kIndex], kIndex});
        if (it > 0)
        {
            v.insert(std::pair{keys[kIndex], kIndex});
        }
    }
}

BENCHMARK_TEMPLATE(BenchErase, sorted_vector_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::unordered_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::sparse_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::ordered_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, boost::container::flat_map<std::string, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchEraseMulti, MultiContainer<std::pair<std::string, Pod>>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, eastl::vector_map<std::string, Pod>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]  = "12345678901234561234567890123456" + std::to_string(_random());
    }
    {

        rusage rUsage{};

        {
            FillRSS(rUsage);
            BOOST_SCOPE_EXIT(rUsage)
            {
                PrintUsage(rUsage);
            }
            BOOST_SCOPE_EXIT_END
            MultiContainer<std::pair<std::string, Pod>> v;
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                v.emplace(std::to_string(_random()), i);
            }
        }
        {
            FillRSS(rUsage);
            BOOST_SCOPE_EXIT(rUsage)
            {
                PrintUsage(rUsage);
            }
            BOOST_SCOPE_EXIT_END
            tsl::sparse_map<std::string, Pod> spp;
            spp.reserve(1024 * 1024);
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                spp.emplace(std::to_string(_random()), i);
            }
        }
        {
            FillRSS(rUsage);
            BOOST_SCOPE_EXIT(rUsage)
            {
                PrintUsage(rUsage);
            }
            BOOST_SCOPE_EXIT_END
            tsl::ordered_map<std::string, Pod> spp;
            spp.reserve(1024 * 1024);
            for (auto i = 0; i < 1024 * 1024; i++)
            {
                spp.emplace(std::to_string(_random()), i);
            }
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}