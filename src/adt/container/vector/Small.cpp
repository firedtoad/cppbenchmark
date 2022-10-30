//
// Created by Administrator on 2022/01/16.
//

#include <benchmark/benchmark.h>
#include <llvm/ADT/SmallVector.h>
#include "llvm/SmallVector.h"
#include "llvm/vector.hpp"
#include <vector>

template <typename V> static void BenchReserve(benchmark::State &state)
{
    auto sz = state.range(0);
    for (auto _ : state)
    {
        V v{};
        v.reserve(sz);
        for (auto i = 0; i < sz; i++)
        {
            v.emplace_back(i);
        }
    }
}

template <typename V> static void BenchReserveSmall(benchmark::State &state)
{
    auto sz = state.range(0);
    for (auto _ : state)
    {
        V v{};
        for (auto i = 0; i < sz; i++)
        {
            v.emplace_back(i);
        }
        benchmark::DoNotOptimize(v);
    }
}
//
//BENCHMARK_TEMPLATE(BenchReserve, std::vector<int>)->Range(65536, 65536);
BENCHMARK_TEMPLATE(BenchReserveSmall, llvm::SmallVector<int, 65536>)->Range(65536, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}