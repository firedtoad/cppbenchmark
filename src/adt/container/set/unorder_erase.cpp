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
#include "unordered_map.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <unordered_set>

#include <llvm/ADT/SparseSet.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/StringSet.h>

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
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, tsl::sparse_set<int>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetInt, llvm::DenseSet<int>);

template <class M> static void BenchEraseSparseSetInt(benchmark::State &state)
{
    M m;
    m.setUniverse(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(i);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = m.erase(r);
        if (it > 0)
        {
            m.insert(r);
        }
    }
}
BENCHMARK_TEMPLATE(BenchEraseSparseSetInt,llvm::SparseSet<unsigned>);

template <class S> static void BenchEraseUnOrderSetString(benchmark::State &state)
{
    S s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        keys[i]   = sKey;
        s.insert(sKey);
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

template <class S> static void BenchEraseCharKeySet(benchmark::State &state)
{
    S s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        keys[i]   = sKey;
        s.insert(sKey);
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
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, tsl::sparse_set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseUnOrderSetString, llvm::DenseSet<llvm::StringRef>);


BENCHMARK_TEMPLATE(BenchEraseCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchEraseCharKeySet, tsl::array_set<char>);


template<class M>
static void BenchEraseStringSet(benchmark::State &state)
{
    M s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        keys[i]   = sKey;
        s.insert(sKey);
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
BENCHMARK_TEMPLATE(BenchEraseStringSet,llvm::StringSet<>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}