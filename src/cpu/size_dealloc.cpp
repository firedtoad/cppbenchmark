#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <memory>

struct S
{
    uint64_t s;
    char     c[50];
};

static void BM_Delete(benchmark::State &state)
{
    int row = state.range(0);

    for (auto _ : state)
    {
        for(auto i=0;i<row;i++)
        {
            auto p=new S[row];
            benchmark::DoNotOptimize(p);
            delete []p;
        }
    }

}

static void BM_DeleteSized(benchmark::State &state)
{
    int row = state.range(0);
    for (auto _ : state)
    {
        for(auto i=0;i<row;i++)
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
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}