//
// Created by Administrator on 2022/01/15.
//

#include <benchmark/benchmark.h>
#include <llvm/ADT/DenseMap.h>
#include <unordered_map>

struct Hasher
{
    size_t operator()(int key) const
    {
        return 1;
    }
};

struct KeyInfo
{
    static inline int getEmptyKey()
    {
        return ~0;
    }
    static inline int getTombstoneKey()
    {
        return ~0 - 1;
    }
    static unsigned getHashValue(const int &Val)
    {
        return 1;
    }
    static bool isEqual(const int &LHS, const int &RHS)
    {
        return LHS == RHS;
    }
};

static void BM_unmap(benchmark::State &state)
{
    std::unordered_map<int, int> unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            unmap[i] = i;
        }
    }
}

static void BM_decay_map(benchmark::State &state)
{
    std::unordered_map<int, int, Hasher> decay_unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            decay_unmap[i] = i;
        }
    }
}

BENCHMARK(BM_unmap);
BENCHMARK(BM_decay_map);

static void BM_DenseMap(benchmark::State &state)
{
    llvm::DenseMap<int, int> unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            unmap[i] = i;
        }
    }
}

static void BM_Decay_DenseMap(benchmark::State &state)
{
    llvm::DenseMap<int, int, KeyInfo> decay_unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            decay_unmap[i] = i;
        }
    }
}

BENCHMARK(BM_DenseMap);
BENCHMARK(BM_Decay_DenseMap);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}