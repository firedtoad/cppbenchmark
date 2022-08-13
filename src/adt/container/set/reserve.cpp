#include <benchmark/benchmark.h>
#include <unordered_set>
#include <llvm/ADT/SparseSet.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SmallSet.h>
#include <llvm/ADT/StringSet.h>

template<class M>
static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
        }
    }
}
template<class M>
static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.reserve(65536);
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
        }
    }
}


BENCHMARK_TEMPLATE(BM_insert,std::unordered_set<int>);
BENCHMARK_TEMPLATE(BM_reserve,std::unordered_set<int>);
BENCHMARK_TEMPLATE(BM_insert,llvm::DenseSet<int>);
BENCHMARK_TEMPLATE(BM_reserve,llvm::DenseSet<int>);
BENCHMARK_TEMPLATE(BM_insert,llvm::SetVector<int>);

template<class M>
static void BM_AdtInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.setUniverse(65536);
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
        }
    }
}

BENCHMARK_TEMPLATE(BM_AdtInsert,llvm::SparseSet<unsigned>);
BENCHMARK_TEMPLATE(BM_AdtInsert,llvm::SparseSet<unsigned>);

template<class M>
static void BM_StringInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(std::to_string(i));
        }
    }
}
BENCHMARK_TEMPLATE(BM_StringInsert,llvm::StringSet<>);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}