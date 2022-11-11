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
#include <algorithm>
#include <benchmark/benchmark.h>
#include <unordered_map>
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
    int second;
    Pod() noexcept = default;
    Pod(uint64_t i_) : i(i_) {}
    Pod(const Pod &pod) noexcept            = default;
    Pod(Pod &&pod) noexcept                 = default;
    Pod &operator=(const Pod &pod) noexcept = default;
    Pod &operator=(Pod &&pod) noexcept      = default;
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

BENCHMARK_TEMPLATE(BenchInsert, sorted_vector<Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::unordered_map<uint64_t, uint64_t>)->Range(1, 1 << 10);

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

BENCHMARK_TEMPLATE(BenchFind, sorted_vector<Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::unordered_map<uint64_t, uint64_t>)->Range(1, 1 << 10);

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
            sum += it.second;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector<Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::unordered_map<uint64_t, uint64_t>)->Range(1, 1 << 10);

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

BENCHMARK_TEMPLATE(BenchErase, sorted_vector<Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::unordered_map<uint64_t, uint64_t>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}