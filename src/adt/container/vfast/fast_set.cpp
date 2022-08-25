//
// Created by Administrator on 2022/07/19.
//

#include "FastVector.h"
#include "parallel_hashmap/phmap.h"
#include "tsl/ordered_set.h"
#include <benchmark/benchmark.h>
#include <llvm/ADT/SetVector.h>
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
    uint64_t i;
    char c[1024];
    Pod() noexcept = default;
    Pod(uint64_t key_) : i(key_) {}
    bool operator==(const Pod &p) const
    {
        return i == p.i;
    }
    bool operator<(const Pod &p) const
    {
        return i < p.i;
    }
    int operator+(int i_) const
    {
        return i + i_;
    }
};

struct PodHash
{
    size_t operator()(const Pod &p) const
    {
        return p.i;
    }
};

struct PodEqual
{
    size_t operator()(const Pod &p1, const Pod &p2) const
    {
        return p1.i == p2.i;
    }
};

struct PodKeyInfo
{
    static inline Pod getEmptyKey()
    {
        return {};
    }
    static inline Pod getTombstoneKey()
    {
        return {};
    }
    static unsigned getHashValue(const Pod &Val)
    {
        return Val.i;
    }
    static bool isEqual(const Pod &LHS, const Pod &RHS)
    {
        return LHS == RHS;
    }
};

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.insert(_random());
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, fast_vector<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, phmap::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, tsl::ordered_set<uint64_t, PodHash, PodEqual>)->Range(1, 1 << 10);

template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.insert(_random() % 65536);
    }
    for (auto _ : state)
    {
        auto it = v.find(_random() % 65536);
        benchmark::DoNotOptimize(it);
    }
}

template <typename V> static void BenchFindSetVector(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.insert(_random() % 65536);
    }
    for (auto _ : state)
    {
        auto it = v[_random() % v.size()];
        benchmark::DoNotOptimize(it);
    }
}

BENCHMARK_TEMPLATE(BenchFind, fast_vector<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, phmap::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFindSetVector, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, tsl::ordered_set<Pod, PodHash, PodEqual>)->Range(1, 1 << 10);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.insert(_random());
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            sum = it + sum;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, fast_vector<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, phmap::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, tsl::ordered_set<uint64_t, PodHash, PodEqual>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.insert(_random());
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
template <typename V> static void BenchEraseSetVector(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.insert(_random() % 65536);
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
BENCHMARK_TEMPLATE(BenchErase, fast_vector<uint32_t, uint32_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, phmap::flat_hash_set<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchEraseSetVector, llvm::SetVector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, tsl::ordered_set<Pod, PodHash, PodEqual>)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}