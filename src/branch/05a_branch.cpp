#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <memory>

#include "benchmark/benchmark.h"

class RandData {
    public:
    RandData(size_t N) : v1(N), v2(N), c1(new bool[N]), c2(new bool[N])
    {
        for (size_t i = 0; i < N; ++i) {
            v1[i] = rand();
            v2[i] = rand();
            c1[i] = rand() & 0x1;
            c2[i] = !c1[i];
        }
    }
    const unsigned long* data1() const { return v1.data(); }
    const unsigned long* data2() const { return v2.data(); }
    const bool* cond1() const { return &c1[0]; }
    const bool* cond2() const { return &c2[0]; }
    private:
    std::vector<unsigned long> v1, v2;
    std::unique_ptr<bool[]> c1, c2;
};

void __attribute__ ((noinline)) f(bool b, unsigned long x, unsigned long& s) { if (b) s += x; }

void BM_branch(benchmark::State& state) {
    srand(1);
    const unsigned int N = state.range(0);
    RandData d(N);
    const unsigned long* const p1 = d.data1();
    const bool* const b1 = d.cond1();
    for (auto _ : state) {
        unsigned long a1 = 0;
        for (size_t i = 0; i < N; ++i) {
            f(b1[i], p1[i], a1);
        }
        benchmark::DoNotOptimize(a1);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N*state.iterations());
}

BENCHMARK(BM_branch)->Arg(1<<22);

BENCHMARK_MAIN();

