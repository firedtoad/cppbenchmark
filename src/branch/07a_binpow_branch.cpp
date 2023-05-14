#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "benchmark/benchmark.h"

inline long binpow(long base, long exponent, long modulus) {
    long res = 1;
    base = base % modulus;
    while (exponent != 0) {
        if (exponent & 1) {
            res = (res * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent >>= 1;
    }
    return res;
}

void BM_branched(benchmark::State& state) {
    srand(1);
    const unsigned int N = state.range(0);
    std::vector<long> v1(N), v2(N), v3(N);
    for (size_t i = 0; i < N; ++i) {
        v1[i] = rand() + 2;
        v2[i] = rand() + 2;
        v3[i] = rand() + 2;
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

BENCHMARK(BM_branched)->Arg(1<<16);

BENCHMARK_MAIN();

