//
// Created by Administrator on 2022/01/15.
//


#include <benchmark/benchmark.h>
#include <iostream>

#include <llvm/ADT/ImmutableSet.h>
#include "tsl/ordered_set.h"
#include "absl/container/btree_set.h"

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

template <class M> static void BenchOrderSetInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(i);
    }
    for (auto _ : state)
    {
        auto c = m.find(_random() % 65536);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderSetInt, std::set<int>);
BENCHMARK_TEMPLATE(BenchOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchOrderSetInt, absl::btree_set<int>);

static void BenchImmutableSetInt(benchmark::State &state)
{
    llvm::ImmutableSet<int>::Factory factory;

    std::vector<int> keys;
    keys.reserve(65536);
    auto m = factory.getEmptySet();
    for (auto i = 0; i < 65536; i++)
    {
        auto r = _random();
        keys.push_back(r);
        m = factory.add(m, r);
    }

    for (auto _ : state)
    {
        auto idx = keys[_random() % 65536];
        auto c   =  m.contains(idx);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BenchImmutableSetInt);


template <class M> static void BenchOrderSetString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]   = std::to_string(_random() % 65536);
        auto sKey = std::to_string(i);
        m.insert(sKey);
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderSetString, std::set<std::string>);
BENCHMARK_TEMPLATE(BenchOrderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchOrderSetString, absl::btree_set<std::string>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
