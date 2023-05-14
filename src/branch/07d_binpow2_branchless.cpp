#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "benchmark/benchmark.h"

inline long binpow(long base, long exponent, long modulus) {
    --modulus;
    long res = 1;
    base = base & modulus;
    while (exponent != 0) {
        long x[2] = { 0, (res * base) & modulus };
        res = x[exponent & 1];
        base = (base * base) & modulus;
        exponent >>= 1;
    }
    return res;
}

void BM_branchless(benchmark::State& state) {
    srand(1);
    const unsigned int N = state.range(0);
    std::vector<long> v1(N), v2(N), v3(N);
    for (size_t i = 0; i < N; ++i) {
        v1[i] = rand() + 2;
        v2[i] = rand() + 2;
        v3[i] = 1UL << ((rand() % 30) + 1);
    }
    long* b = v1.data();
    long* e = v2.data();
    long* m = v3.data();
    for (auto _ : state) {
        for (size_t i = 0; i < N; ++i) {
            benchmark::DoNotOptimize(binpow(b[i], e[i], m[i]));
        }
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N*state.iterations());
}

BENCHMARK(BM_branchless)->Arg(1<<16);

BENCHMARK_MAIN();

