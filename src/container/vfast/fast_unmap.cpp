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

#include "container/FastVector.h"
#include "flat_hash_map/bytell_hash_map.hpp"
#include "flat_hash_map/flat_hash_map.hpp"
#include "tsl/robin_map.h"
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
    uint64_t key1;
    int second;
    char c[1024];
    Pod() noexcept = default;
    Pod(uint64_t key_) : key1(key_) {}
    Pod(const Pod &pod) noexcept            = default;
    Pod(Pod &&pod) noexcept                 = default;
    Pod &operator=(const Pod &pod) noexcept = default;
    Pod &operator=(Pod &&pod) noexcept      = default;
    int operator+(int i) const
    {
        return key1 + i;
    }
};

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v[r]   = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, fast_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, ska::flat_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, ska::bytell_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::unordered_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::robin_map<uint64_t, Pod>)->Range(1, 1 << 10);

template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
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

BENCHMARK_TEMPLATE(BenchFind, fast_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, ska::flat_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, ska::bytell_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::unordered_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::robin_map<uint64_t, Pod>)->Range(1, 1 << 10);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
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
            sum = it.second + sum;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, fast_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, ska::flat_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, ska::bytell_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::unordered_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::robin_map<uint64_t, Pod>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
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

BENCHMARK_TEMPLATE(BenchErase, fast_vector_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, ska::flat_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, ska::bytell_hash_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::unordered_map<uint64_t, Pod>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::robin_map<uint64_t, Pod>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}