#include <benchmark/benchmark.h>
#include <unordered_map>

#include <llvm/ADT/IntervalMap.h>
static void BM_Interval(benchmark::State &state)
{
    for (auto _ : state)
    {
        llvm::IntervalMap<int,int>::Allocator alloc;
        llvm::IntervalMap<int,int> m(alloc);
        for (auto i = 0; i < state.range(0); i+=16)
        {
            m.insert(i,i+16,10);
        }
    }
}


BENCHMARK(BM_Interval)->Range(1,1<<20);

int main(int argc, char **argv)
{


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}