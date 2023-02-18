#include <benchmark/benchmark.h>
#include <vector>

static void BM_ONE(benchmark::State &state)
{
    std::vector<char> va(state.range(0) * 64);
    for (auto _ : state)
    {
        for (auto &it : va)
        {
            benchmark::DoNotOptimize(it);
        }
    }
}

BENCHMARK(BM_ONE)->Range(1,1<<20);

static void BM_TWO(benchmark::State &state)
{
    std::vector<std::vector<char>> vd(state.range(0), std::vector<char>(64));
    for (auto _ : state)
    {
        for (auto &it : vd)
        {
            for (auto &id : it)
            {
                benchmark::DoNotOptimize(id);
            }
        }
    }
}
BENCHMARK(BM_TWO)->Range(1,1<<20);;

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}