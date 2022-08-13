//
// Created by Administrator on 2022/07/19.
//

#include "FastVector.h"
#include "bytell_hash_map.hpp"
#include "flat_hash_map.hpp"
#include "tsl/robin_set.h"
#include <benchmark/benchmark.h>
#include <llvm/ADT/SetVector.h>
#include <unordered_set>
static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 103456789, y = 362436069, z = 521088629;
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

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, fast_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::unordered_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, ska::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, ska::bytell_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::robin_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchInsertVecSet(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            auto r = _random();
            v.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsertVecSet, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto it = v.find(_random() % 65536);
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchFind, fast_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::unordered_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, ska::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, ska::bytell_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::robin_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchFindVecSet(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto it = v[_random() % v.size()];
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchFindVecSet, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            sum += it;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, fast_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::unordered_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, ska::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, ska::bytell_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::robin_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchRangeVecSet(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            sum += it;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRangeVecSet, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.erase(r);
        if (it > 0)
        {
            v.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchErase, fast_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::unordered_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, ska::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, ska::bytell_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::robin_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchEraseVecSet(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v.insert(r);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.remove(r);
        if (it > 0)
        {
            v.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseVecSet, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}