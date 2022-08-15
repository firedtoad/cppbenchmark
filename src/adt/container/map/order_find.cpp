//
// Created by Administrator on 2022/01/15.
//

#include "absl/container/btree_map.h"
#include "tsl/ordered_map.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <map>
#include <llvm/ADT/ImmutableMap.h>

static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 521288629;
    uint64_t        t;

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

template <class M> static void BenchOrderMapInt(benchmark::State &state)
{
    M                m;
    std::vector<int> keys;
    keys.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto r = _random();
        keys.push_back(r);
        m[r] = i;
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % 65536];
        auto c   = m.find(idx);
        auto v   = c->second;
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(
    BenchOrderMapInt,
    tsl::ordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<int, int>>, std::vector<std::pair<int, int>>>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, absl::btree_map<int, int>);

static void BenchImmutableMapInt(benchmark::State &state)
{
    llvm::ImmutableMap<int, int>::Factory factory;

    std::vector<int> keys;
    keys.reserve(65536);
    auto m = factory.getEmptyMap();
    for (auto i = 0; i < 65536; i++)
    {
        auto r = _random();
        keys.push_back(r);
        m = factory.add(m, r, i);
    }

    for (auto _ : state)
    {
        auto idx = keys[_random() % 65536];
        auto c   =  m.lookup(idx);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BenchImmutableMapInt);

template <class M> static void BenchOrderMapString(benchmark::State &state)
{
    M                        m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" + std::to_string(_random());
        m[keys[i].c_str()] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, tsl::ordered_map<std::string, int, std::hash<std::string>, std::equal_to<std::string>,
                                                         std::allocator<std::pair<std::string, int>>, std::vector<std::pair<std::string, int>>>);
BENCHMARK_TEMPLATE(BenchOrderMapString, absl::btree_map<std::string, int>);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}