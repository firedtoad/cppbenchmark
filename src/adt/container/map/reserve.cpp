#include <benchmark/benchmark.h>
#include <unordered_map>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/IndexedMap.h>
#include <llvm/ADT/StringMap.h>

template<class  M>
static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (auto i = 0; i < state.range(0); i++)
        {
            m[i] = i;
        }
    }
}


BENCHMARK_TEMPLATE(BM_insert,llvm::DenseMap<int,int>)->Range(1,1<<20);
BENCHMARK_TEMPLATE(BM_insert,llvm::MapVector<int,int>)->Range(1,1<<20);

template<class  M>
static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_reserve,llvm::DenseMap<int,int>)->Range(1,1<<20);
BENCHMARK_TEMPLATE(BM_reserve,llvm::MapVector<int,int>)->Range(1,1<<20);
template<class  M>
static void BM_reserveIndex(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_reserveIndex,llvm::IndexedMap<int>)->Range(1,1<<20);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}