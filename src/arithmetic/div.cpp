//
// Created by Administrator on 2022/01/05.
//

#include<benchmark/benchmark.h>
#include<stdint.h>

const double bp_double = 1000000;
const int32_t bp_int32 = 1000000;
const int64_t bp_int64 = 1000000;
__attribute__((noinline))
double calc(double x, double y) {
  return x / bp_double - y / bp_double;
}
__attribute__((noinline))
double calc1(double x, double y) {
  return (x - y) / bp_double;
}
__attribute__((noinline))
int calc_32(int x, int y) {
  return x / bp_int32 - y / bp_int32;
}
__attribute__((noinline))
int calc1_32(int x, int y) {
  return (x - y) / bp_int32;
}
__attribute__((noinline))
int64_t calc_64(int64_t x, int64_t y) {
  return x / bp_int64 - y / bp_int64;
}
__attribute__((noinline))
int64_t calc1_64(int64_t x, int64_t y) {
  return (x - y) / bp_int64;
}

static void calc(benchmark::State &state) {
  double d = 1;
  for (auto _ : state) {
    benchmark::DoNotOptimize(calc(d + 100000, d));
    d++;
  }
}

BENCHMARK(calc);

static void calc1(benchmark::State &state) {
  double d = 1;
  for (auto _ : state) {
    benchmark::DoNotOptimize(calc1(d + 100000, d));
    d++;
  }
}

BENCHMARK(calc1);

static void calc_int(benchmark::State &state) {
  int d = 1;
  for (auto _ : state) {
    benchmark::DoNotOptimize(calc_32(d + 100000, d));
    d++;
  }
}

BENCHMARK(calc_int);

static void calc1_int(benchmark::State &state) {
  int d = 1;
  for (auto _ : state) {
    benchmark::DoNotOptimize(calc1_32(d + 100000, d));
    d++;
  }
}

BENCHMARK(calc1_int);

static void calc_int64(benchmark::State &state) {
  int64_t d = 1;
  for (auto _ : state) {
    benchmark::DoNotOptimize(calc_64(d + 100000, d));
    d++;
  }
}

BENCHMARK(calc_int64);

static void calc1_int64(benchmark::State &state) {
  int64_t d = 1;
  for (auto _ : state) {
    benchmark::DoNotOptimize(calc1_64(d + 100000, d));
    d++;
  }
}

BENCHMARK(calc1_int64);

BENCHMARK_MAIN();
