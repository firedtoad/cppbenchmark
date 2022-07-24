#include <benchmark/benchmark.h>
#include <unordered_set>

static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::unordered_set<int> m;
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
        }
    }
}

static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::unordered_set<int> m;
        m.reserve(65536);
        for (auto i = 0; i < 65536; i++)
        {
            m.insert(i);
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