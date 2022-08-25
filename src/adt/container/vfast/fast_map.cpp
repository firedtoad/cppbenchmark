//
// Created by Administrator on 2022/07/19.
//

#include "FastVector.h"
#include "parallel_hashmap/phmap.h"
#include <benchmark/benchmark.h>
#include <llvm/ADT/MapVector.h>
#include <unordered_map>

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
    int i;
    Pod() noexcept = default;
    Pod(uint64_t i_) : i(i_) {}
    Pod(const Pod &pod) noexcept = default;
    Pod(Pod &&pod) noexcept      = default;
    Pod &operator=(const Pod &pod) noexcept = default;
    Pod &operator=(Pod &&pod) noexcept = default;
    bool operator<(const Pod &p) const
    {
        return i < p.i;
    }
    int operator+(int i_) const
    {
        return i + i_;
    }
};

template <typename V> static void BenchInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        //        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            uint64_t r = _random();
            v[r]       = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert, fast_vector_map<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchInsert, fast_vector_map<uint64_t, uint64_t, llvm::DenseMap<uint64_t, uint32_t>>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchInsert, llvm::MapVector<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);

template <typename V> static void BenchFind(benchmark::State &state)
{
    V v;
    std::vector<int> keys;
    v.reserve(state.range(0));
    keys.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        keys.push_back(r);
        v[r] = i;
    }
    for (auto _ : state)
    {
        auto idx = keys[_random() % state.range(0)];
        auto c   = v.find(idx);
        auto v   = c->second;
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK_TEMPLATE(BenchFind, fast_vector_map<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchFind, fast_vector_map<uint64_t, uint64_t, llvm::DenseMap<uint64_t, uint32_t>>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchFind, llvm::MapVector<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);

template <typename V> static void BenchRange(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random();
        v[r]   = r;
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            sum = it.second + sum;
        }
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, fast_vector_map<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchRange, fast_vector_map<uint64_t, uint64_t, llvm::DenseMap<uint64_t, uint32_t>>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchRange, llvm::MapVector<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);

template <typename V> static void BenchErase(benchmark::State &state)
{
    V v;
    for (auto i = 0; i < state.range(0); i++)
    {
        auto r = _random() % 65536;
        v[r]   = r;
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = v.erase(r);
        if (it > 0)
        {
            v[r] = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchErase, fast_vector_map<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchErase, fast_vector_map<uint64_t, uint64_t, llvm::DenseMap<uint64_t, uint32_t>>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BenchErase, llvm::MapVector<uint64_t, uint64_t>)->Range(1 << 10, 1 << 20);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}