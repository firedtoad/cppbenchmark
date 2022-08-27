//
// Created by Administrator on 2022/01/15.
//
#include "bytell_hash_map.hpp"
#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/array-hash/array_set.h"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include "tsl/htrie_set.h"
#include "tsl/robin_set.h"
#include "tsl/sparse_set.h"
#include "absl/container/flat_hash_set.h"
#include "unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <unordered_set>

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

template <class S> static void BenchEraseUnOrderSetInt(benchmark::State &state)
{
    S s;
    s.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        s.insert(i);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, ska::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, ska::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, ska::bytell_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, phmap::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, absl::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::sparse_set<int>);

template <class S> static void BenchEraseUnOrderSetString(benchmark::State &state)
{
    S s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" +std::to_string(_random());
        s.insert(keys[i]);
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

template <class S> static void BenchRangeCharKeySet(benchmark::State &state)
{
    S s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" +std::to_string(_random());
        s.insert(keys[i]);
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, std::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, ska::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, ska::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, ska::bytell_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, phmap::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, absl::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::sparse_set<std::string>);

BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::array_set<char>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}
