#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "benchmark/benchmark.h"

void BM_branchless(benchmark::State& state) {
    srand(1);
    const unsigned int N = state.range(0);
    std::vector<unsigned long> v1(N), v2(N);
    std::vector<int> c1(N);
    for (size_t i = 0; i < N; ++i) {
        v1[i] = rand();
        v2[i] = rand();
        c1[i] = rand() > 0;
    }
    unsigned long* p1 = v1.data();
    unsigned long* p2 = v2.data();
    int* b1 = c1.data();
    for (auto _ : state) {
        unsigned long a1 = 0, a2 = 0;
        for (size_t i = 0; i < N; ++i) {
            unsigned long s1[2] = {             0, p1[i] - p2[i] };
            unsigned long s2[2] = { p1[i] * p2[i],             0 };
            a1 += s1[bool(b1[i])];
            a2 += s2[bool(b1[i])];
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N*state.iterations());
}

BENCHMARK(BM_branchless)->Arg(1<<22);

BENCHMARK_MAIN();
