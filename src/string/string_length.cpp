#include <benchmark/benchmark.h>
#include <cstring>
#include <iostream>
#include <numeric>

std::string tag  = "1234567890123456";

template <typename Ch>
inline size_t StrLen(const Ch* s) {
    const Ch* p = s;
    while (*p) ++p;
    return size_t(p - s);
}

template <typename Ch>
inline size_t WrapStrLen(const Ch* s) {
    return strlen(s);
}

static void BenchNaiveStrlen(benchmark::State &state)
{
    tag.resize(state.range(0));
    std::iota(tag.begin(),tag.end(),1);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(StrLen(tag.c_str()));
    }
}
BENCHMARK(BenchNaiveStrlen)->Range(1,8192);;

static void BenchSysStrlen(benchmark::State &state)
{
    tag.resize(state.range(0));
    std::iota(tag.begin(),tag.end(),1);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(strlen(tag.c_str()));
    }
}

BENCHMARK(BenchSysStrlen)->Range(1,8192);

static void BenchWrapStrLen(benchmark::State &state)
{
    tag.resize(state.range(0));
    std::iota(tag.begin(),tag.end(),1);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(WrapStrLen(tag.c_str()));
    }
}

BENCHMARK(BenchWrapStrLen)->Range(1,8192);


int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}