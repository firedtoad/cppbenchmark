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

#include <benchmark/benchmark.h>
#include <set>
#include <memory_resource>
#include <numeric>

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

template <class M> static void BM_Insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (auto i = 0; i < state.range(0); i++)
        {
            m.insert(i);
        }
    }
}

template <class M> static void BM_PMR_Insert(benchmark::State &state)
{
    std::vector<uint8_t> vec(state.range(0)*40);
    std::pmr::monotonic_buffer_resource pool(vec.data(),vec.size());
    for (auto _ : state)
    {
        M m{&pool};
        for (auto i = 0; i < state.range(0); i++)
        {
            m.insert(i);
        }
    }
}
BENCHMARK_TEMPLATE(BM_Insert, std::set<int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_PMR_Insert, std::pmr::set<int>)->Range(1 << 10, 1 << 20);



template <class M> static void BM_Range(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < state.range(0); i++)
    {
        m.insert(i);
    }
    for (auto _ : state)
    {
        auto r= std::accumulate(m.begin(),m.end(),0);
        benchmark::DoNotOptimize(r);
    }
}

template <class M> static void BM_PMR_Range(benchmark::State &state)
{
    std::vector<uint8_t> vec(state.range(0)*40);
    std::pmr::monotonic_buffer_resource pool(vec.data(),vec.size());
    M m{&pool};
    for (auto i = 0; i < state.range(0); i++)
    {
        m.insert(i);
    }
    for (auto _ : state)
    {
        auto r= std::accumulate(m.begin(),m.end(),0);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BM_Range, std::set<int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_PMR_Range, std::pmr::set<int>)->Range(1 << 10, 1 << 20);

template <class M> static void BM_Find(benchmark::State &state)
{
    M m;
    std::vector<int> keys;
    keys.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        keys.push_back(r);
        m.insert(r);
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % state.range(0)];
        auto c   = m.find(idx);
        benchmark::DoNotOptimize(c);
    }
}

template <class M> static void BM_PMR_Find(benchmark::State &state)
{
    std::vector<uint8_t> vec(state.range(0)*40);
    std::pmr::monotonic_buffer_resource pool(vec.data(),vec.size());
    M m{&pool};
    std::vector<int> keys;
    keys.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        keys.push_back(r);
        m.insert(r);
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % state.range(0)];
        auto c   = m.find(idx);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BM_Find, std::set<int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_PMR_Find, std::pmr::set<int>)->Range(1 << 10, 1 << 20);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}