#include <benchmark/benchmark.h>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

struct S
{
    uint64_t s;
    //    __attribute__((always_inline)) void operator delete[] (void * p,size_t sz){
    //        operator delete(p);
    //    }
};

static void BM_Delete(benchmark::State &state)
{
    int row = state.range(0);

    for (auto _ : state)
    {
        for (auto i = 0; i < row; i++)
        {
            auto p = new S[row];
            benchmark::DoNotOptimize(p);
            delete[] p;
        }
    }
}

static void BM_DeleteSized(benchmark::State &state)
{
    int row = state.range(0);
    for (auto _ : state)
    {
        for (auto i = 0; i < row; i++)
        {
            std::vector<S> p;
            p.reserve(row);
            benchmark::DoNotOptimize(p);
        }
    }
}

BENCHMARK(BM_Delete)->Range(1, 1 << 20);
BENCHMARK(BM_DeleteSized)->Range(1, 1 << 20);

int main(int argc, char **argv)
{
    auto p = new S[1];
    benchmark::DoNotOptimize(p);
    delete[] p;
    std::map<int, int> mp;
    std::set<int> st;
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}