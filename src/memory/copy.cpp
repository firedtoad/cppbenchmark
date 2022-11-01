#include <benchmark/benchmark.h>
#include <cstring>
#include <vector>
#include <openssl/rc5.h>
static void BenchMemCpy(benchmark::State &state)
{
    std::vector<char> cache;
    cache.resize(state.range(0));
    std::vector<char> dst;
    dst.resize(state.range(0));
    for (auto _ : state)
    {
        memcpy(dst.data(),cache.data(),cache.size());
        benchmark::DoNotOptimize(dst.size());
    }
}

static void BenchStdCopy(benchmark::State &state)
{
    std::vector<char> cache;
    cache.resize(state.range(0));
    std::vector<char> dst;
    dst.resize(state.range(0));
    for (auto _ : state)
    {
        std::copy(cache.begin(),cache.end(),dst.data());
        benchmark::DoNotOptimize(dst.size());
    }
}

BENCHMARK(BenchMemCpy)->Range(1,1024);
BENCHMARK(BenchStdCopy)->Range(1,1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}