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
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/StringSet.h>
#include <llvm/ADT/FoldingSet.h>

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

template <class M> static void BenchUnOrderSetInt(benchmark::State &state)
{
    M m;
    m.reserve(65536);
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

BENCHMARK_TEMPLATE(BenchUnOrderSetInt, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, ska::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, ska::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, ska::bytell_hash_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, phmap::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt, tsl::sparse_set<int>);
BENCHMARK_TEMPLATE(BenchUnOrderSetInt,llvm::DenseSet<int>);





template <class M> static void BenchUnOrderVecSetInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(i);
    }
    for (auto _ : state)
    {
        auto c = m[_random() % 65536];
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderVecSetInt,llvm::SetVector<int>);

template <class M> static void BenchUnOrderSparseSetInt(benchmark::State &state)
{
    M m;
    m.setUniverse(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(i);
    }
    for (auto _ : state)
    {
        auto c = m[_random() % 65536];
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderSparseSetInt,llvm::SparseSet<unsigned>);

template <class M> static void BenchUnOrderSetString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    std::vector<std::string> keys(65536);
    int                      k = 1000000;
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" + std::to_string(k++);
        m.insert(keys[i]);
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}
template <class M> static void BenchCharKeySet(benchmark::State &state)
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
        auto c      = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderSetString, std::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, ska::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, ska::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, ska::bytell_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, phmap::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, tsl::sparse_set<std::string>);
BENCHMARK_TEMPLATE(BenchUnOrderSetString, llvm::DenseSet<llvm::StringRef>);


BENCHMARK_TEMPLATE(BenchCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchCharKeySet, tsl::array_set<char>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    tsl::array_map<char, int> m;
    return 0;
}
