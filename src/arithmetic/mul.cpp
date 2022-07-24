#include <benchmark/benchmark.h>
#include <stdint.h>

__attribute__((noinline)) float calc_float(float x, float y)
{
    return x * y;
}

__attribute__((noinline)) double calc_double(double x, double y)
{
    return x * y;
}

__attribute__((noinline)) int calc_32(int x, int y)
{
    return x * y;
}

__attribute__((noinline)) int64_t calc_64(int64_t x, int64_t y)
{
    return x * y;
}

static void calc_int(benchmark::State &state)
{
    int d = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(calc_32(d + 100000, d));
        d++;
    }
}

BENCHMARK(calc_int);

static void calc_int64(benchmark::State &state)
{
    int64_t d = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(calc_64(d + 100000, d));
        d++;
    }
}

BENCHMARK(calc_int64);

static void calc_float(benchmark::State &state)
{
    float d = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(calc_float(d + 100000, d));
        d++;
    }
}

BENCHMARK(calc_float);

static void calc_double(benchmark::State &state)
{
    double d = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(calc_double(d + 100000, d));
        d++;
    }
}

BENCHMARK(calc_double);

BENCHMARK_MAIN();
