#include <benchmark/benchmark.h>
#include <vector>
static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<int> v;
        for (auto i = 0; i < 1024; i++)
        {
            v.push_back(i);
        }
    }
}

static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<int> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.push_back(i);
        }
    }
}

BENCHMARK(BM_insert);
BENCHMARK(BM_reserve);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}