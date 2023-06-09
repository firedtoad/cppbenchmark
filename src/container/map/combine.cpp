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
#include <map>
#include <parallel_hashmap/phmap.h>
#include <sstream>
#include <unordered_map>
#include "tsl/ordered_map.h"

static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 521288629;
    uint64_t t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline uint64_t _random()
{
    return xor_shift96();
}

std::vector<uint64_t> keys(65536);
template <class M> static void BenchCombineMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % 65536];
        auto c   = m.find(idx);
        auto v   = c->second;
        benchmark::DoNotOptimize(v);
    }
}

template <class M> static void BenchTwiceMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        auto r     = keys[i];
        uint32_t f = r >> 32;
        uint32_t s = r & 0xFFFFFFFF;
        m[f][s]    = i;
    }
    for (auto _ : state)
    {
        auto idx   = keys[_random() % 65536];
        uint32_t f = idx >> 32;
        uint32_t s = idx & 0xFFFFFFFF;

        auto c = m.find(f);
        if (c != m.end())
        {
            auto d = c->second.find(s);
            auto v = d->second;
            benchmark::DoNotOptimize(v);
        }
    }
}

BENCHMARK_TEMPLATE(BenchCombineMapInt, std::map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, std::map<uint32_t, std::map<uint32_t, uint32_t>>);

BENCHMARK_TEMPLATE(BenchCombineMapInt, std::unordered_map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>>);

BENCHMARK_TEMPLATE(BenchCombineMapInt, phmap::flat_hash_map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, phmap::flat_hash_map<uint32_t, phmap::flat_hash_map<uint32_t, uint32_t>>);

BENCHMARK_TEMPLATE(BenchCombineMapInt, tsl::ordered_map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, tsl::ordered_map<uint32_t, tsl::ordered_map<uint32_t, uint32_t>>);

BENCHMARK_TEMPLATE(BenchCombineMapInt, tsl::vector_map<uint64_t, uint64_t>);
BENCHMARK_TEMPLATE(BenchTwiceMapInt, tsl::vector_map<uint32_t, tsl::vector_map<uint32_t, uint32_t>>);

int main(int argc, char **argv)
{
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = _random();
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}