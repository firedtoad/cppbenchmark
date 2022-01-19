//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include "SmallVector.h"
#include "vector.hpp"
#include <vector>

template<typename V>
static void BenchInsert(benchmark::State &state) {
    for (auto _ : state) {
        V v;
        for (auto i = 0; i < state.range(0); i++) {
            v.push_back(i);
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsert,std::vector<int>)->Range(2,1024);
BENCHMARK_TEMPLATE(BenchInsert,llvm::SmallVector<int,1024>)->Range(2,1024);
BENCHMARK_TEMPLATE(BenchInsert,llvm_vecsmall::SmallVector<int,1024>)->Range(2,1024);

template<typename V>
static void BenchReserve(benchmark::State &state) {
    for (auto _ : state) {
        V v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++) {
            v.push_back(i);
        }
    }
}

BENCHMARK_TEMPLATE(BenchReserve,std::vector<int>)->Range(2,1024);
BENCHMARK_TEMPLATE(BenchReserve,llvm::SmallVector<int,1024>)->Range(2,1024);
BENCHMARK_TEMPLATE(BenchReserve,llvm_vecsmall::SmallVector<int,1024>)->Range(2,1024);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}