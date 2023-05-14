#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <memory>

#include "benchmark/benchmark.h"

int __attribute__ ((noinline)) f1(int i) { return -i; }
int __attribute__ ((noinline)) f2(int i) { return i + 1; }

void BM_branch(benchmark::State& state) {
    srand(1);
    const unsigned int N = state.range(0);
    std::unique_ptr<bool[]> b1(new bool[N]);
    std::vector<int> v1(N);
    for (size_t i = 0; i < N; ++i) {
        v1[i] = rand();
        b1[i] = rand() & 0x1;
    }
    const bool* const b = &b1[0];
    const int* const v = &v1[0];
    for (auto _ : state) {
        unsigned long a1 = 0;
        for (size_t i = 0; i < N; ++i) {
            if (b[i]) {
                a1 += f1(v[i]);
            } else {
                a1 += f2(v[i]);
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N*state.iterations());
}

BENCHMARK(BM_branch)->Arg(1<<22);

BENCHMARK_MAIN();

