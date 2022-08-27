//
// Created by Administrator on 2022/01/15.
//
#include "bytell_hash_map.hpp"
#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_map.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/htrie_map.h"
#include "tsl/robin_map.h"
#include "tsl/sparse_map.h"
#include "unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <absl/container/flat_hash_map.h>
#include <iostream>
#include <unordered_map>

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

template <class M> static void BenchEraseUnOrderMapInt(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, ska::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, ska::bytell_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, absl::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, robin_hood::unordered_flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, spp::sparse_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::bhopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::hopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::robin_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapInt, tsl::sparse_map<int, int>);

template <class M> static void BenchEraseUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey ="12345678901234561234567890123456" + std::to_string(_random());
        keys[i]   = sKey;
        m[sKey]   = i;
    }
    int r{};
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = 0;
        }
    }
    benchmark::DoNotOptimize(r);
}

template <class M> static void BenchEraseCharKeyMap(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey ="12345678901234561234567890123456" + std::to_string(_random());
        keys[i]   = sKey;
        m[sKey]   = i;
    }
    int r{};
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = 0;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, absl::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseCharKeyMap, tsl::htrie_map<char, int>);
BENCHMARK_TEMPLATE(BenchEraseCharKeyMap, tsl::array_map<char, int>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
