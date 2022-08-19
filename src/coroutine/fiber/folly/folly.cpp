#include <benchmark/benchmark.h>
#include <folly/fibers/Fiber.h>
#include <iostream>


static void BenchFiberCreate(benchmark::State &state)
{
    for (auto _ : state)
    {

    }
}

BENCHMARK(BenchFiberCreate)->Range(1024, 1024);

struct alignas(64) SP
{
    int x;
    int y;
    int z;
    int w;
    int k;
    std::string s;
};

int main(int argc, char **argv)
{
    std::cout<<alignof(SP)<<'\n';
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
