#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "benchmark/benchmark.h"

void BM_branch_predict(benchmark::State& state) {
    srand(1);
    const unsigned int N = state.range(0);
    std::vector<unsigned long> v1(N), v2(N);
    std::vector<int> c1(N);
    for (size_t i = 0; i < N; ++i) {
        v1[i] = rand();
        v2[i] = rand();
        if (i == 0) c1[i] = rand() >= 0; else c1[i] = !c1[i - 1];
    }
    unsigned long* p1 = v1.data();
    unsigned long* p2 = v2.data(); (void)p2;
    int* b1 = c1.data();
    for (auto _ : state) {
        unsigned long a1 = 0, a2 = 0;
        for (size_t i = 0; i < N; ++i) {
            if (b1[i]) {
                a1 += p1[i];
            } else {
                a1 *= p2[i];
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N*state.iterations());
}

BENCHMARK(BM_branch_predict)->Arg(1<<22);

BENCHMARK_MAIN();

