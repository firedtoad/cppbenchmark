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
#include <iostream>
#include <unordered_map>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/IndexedMap.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/StringMap.h>

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long        t;

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

template <class M> static void BenchUnOrderMapInt(benchmark::State &state)
{
    M                m;
    std::vector<int> keys;
    m.reserve(65536);
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

BENCHMARK_TEMPLATE(BenchUnOrderMapInt, std::unordered_map<int, int>);

BENCHMARK_TEMPLATE(BenchUnOrderMapInt, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, ska::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, ska::bytell_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, robin_hood::unordered_flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, spp::sparse_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::bhopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::hopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::robin_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::sparse_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, llvm::DenseMap<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, llvm::MapVector<int, int>);

template <class M> static void BenchIndexedMap(benchmark::State &state)
{
    M                m;
    std::vector<int> keys;
    m.resize(65536);
    keys.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto r = _random();
        keys.push_back(r % 65536);
        m[i] = r;
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % 65536];
        auto c   = m[idx];
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchIndexedMap, llvm::IndexedMap<int>);

template <class M> static void BenchUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    std::vector<std::string> keys(65536);
    int                      k = 1000000;
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" + std::to_string(k++);
        m[keys[i].c_str()] = i;
    }

    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

template <class M> static void BenchCharKeyMap(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    int                      k = 1000000;
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" + std::to_string(k++);
        m[keys[i].c_str()] = i;
    }

    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, llvm::DenseMap<llvm::StringRef, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, llvm::MapVector<llvm::StringRef, int>);

template <class M> static void BenchStringMapNoSSO(benchmark::State &state)
{
    M                        m;
    std::vector<std::string> keys(65536);
    int                      k = 1000000;
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" + std::to_string(k++);
        m[keys[i].c_str()] = i;
    }

    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchStringMapNoSSO, llvm::StringMap<int>);

template <class M> static void BenchIndexedMapString(benchmark::State &state)
{
    M                m;
    std::vector<int> keys;
    m.resize(65536);
    keys.reserve(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto r   = _random();
        auto val = "12345678901234561234567890123456" + std::to_string(r);
        keys.push_back(r % 65536);
        m[i] = val;
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % 65536];
        auto c   = m[idx];
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchIndexedMapString, llvm::IndexedMap<std::string>);

BENCHMARK_TEMPLATE(BenchCharKeyMap, tsl::htrie_map<char, int>);
BENCHMARK_TEMPLATE(BenchCharKeyMap, tsl::array_map<char, int>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
