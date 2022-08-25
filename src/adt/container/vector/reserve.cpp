#include <benchmark/benchmark.h>
#include <llvm/ADT/SmallVector.h>
#include <vector>

struct Pod
{
    char c[128];
};

static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        llvm::SmallVector<Pod, 1024> v;
        for (auto i = 0; i < 1024; i++)
        {
            benchmark::DoNotOptimize(v.emplace_back(Pod{}));
        }
    }
}

static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<Pod> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            benchmark::DoNotOptimize(v.emplace_back(Pod{}));
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