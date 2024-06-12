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
    uint64_t first;
    uint64_t second;
    Pod() noexcept = default;
    Pod(uint64_t i, uint64_t j) : first(i), second(j) {}
    bool operator<(const Pod &p) const
    {
        return first < p.first;
    }
};

struct PodTag
{
};

struct PodExtractor
{
    using result_type = uint64_t;

    result_type operator()(const Pod &xData) const
    {
        return xData.second;
        //        return -1;
    }
};

struct PodExtractorDecay
{
    using result_type = uint64_t;

    result_type operator()(const Pod &xData) const
    {
        return -1;
    }
};

template <typename V>
using MultiHashContainer = boost::multi_index::multi_index_container<
    V, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::member<V, uint64_t, &V::first>>,
                                      boost::multi_index::ordered_non_unique<boost::multi_index::tag<PodTag>, PodExtractor>>>;

template <typename V>
using MultiHashContainerDecay = boost::multi_index::multi_index_container<
    V, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::member<V, uint64_t, &V::first>>,
                                      boost::multi_index::ordered_non_unique<boost::multi_index::tag<PodTag>, PodExtractorDecay>>>;

template <typename V> static void BenchInsertMulti(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v.insert({r, r});
        }
    }
}

// BENCHMARK_TEMPLATE(BenchInsertMulti, MultiContainer<Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchInsertMulti, MultiHashContainer<Pod>)->Range(1, 1 << 16);
// BENCHMARK_TEMPLATE(BenchInsertMulti, MultiHashContainerDecay<Pod>)->Range(1, 1 << 16);
std::vector<uint64_t> keys;
static MultiHashContainer<Pod> multiHash;
static void BenchModifyMultiHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto index = _random() % keys.size();
        auto r     = keys[index];
        auto it    = multiHash.find(r);
        if (it != multiHash.end())
        {
            multiHash.modify(it, [&](auto &xData)
                             {
                                 xData.second = r;
                             });
        }
        benchmark::DoNotOptimize(it);
    }
}

static MultiHashContainer<Pod> multiHashDecay;
static void BenchModifyMultiHashDecay(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto index = _random() % keys.size();
        auto r     = keys[index];
        auto it    = multiHashDecay.find(r);
        if (it != multiHashDecay.end())
        {
            multiHashDecay.modify(it, [&](auto &xData)
                                  {
                                      xData.second = r;
                                  });
        }
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK(BenchModifyMultiHash)->Range(1, 1 << 1);
BENCHMARK(BenchModifyMultiHashDecay)->Range(1, 1 << 1);

int main(int argc, char **argv)
{
    keys.reserve(1 << 18);
    for (auto i = 0; i < 1 << 18; i++)
    {
        auto r = _random();
        keys.emplace_back(r);
        multiHash.insert({r, r});
        multiHashDecay.insert({r, r});
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}