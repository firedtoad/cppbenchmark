//
// Created by Administrator on 2022/01/05.
//

#include<benchmark/benchmark.h>
#include<stdint.h>

float calc32(float x, float y) {
    return x / y;
}

double calc(double x, double y) {
    return x / y;
}

uint32_t calc_int32(uint32_t x, uint32_t y) {
    return x / y;
}

int64_t calc_64(uint64_t x, uint64_t y) {
    return x / y;
}

static void calc_float(benchmark::State &state) {
    float p = state.range(0);
    for (auto _ : state) {
        for (float d = 1; d < p; d++) {
            benchmark::DoNotOptimize(calc32(p, d));
        }
    }
}

BENCHMARK(calc_float)->Range(10000, 10000);

static void calc_double(benchmark::State &state) {
    double p = state.range(0);
    for (auto _ : state) {
        for (double d = 1; d < p; d++) {
            benchmark::DoNotOptimize(calc(p, d));
        }
    }
}

BENCHMARK(calc_double)->Range(10000, 10000);


static void calc_uint32(benchmark::State &state) {
    uint32_t p = state.range(0);
    for (auto _ : state) {
        for (uint32_t d = 1; d < p; d++) {
            benchmark::DoNotOptimize(calc_int32(p, d));
        }
    }
}

BENCHMARK(calc_uint32)->Range(10000, 10000);

static void calc_uint64(benchmark::State &state) {
    uint64_t p = state.range(0);
    for (auto _ : state) {
        for (uint64_t d = 1; d < p; d++) {
            benchmark::DoNotOptimize(calc_64(p, d));
        }
    }
}

BENCHMARK(calc_uint64)->Range(10000, 10000);

BENCHMARK_MAIN();
