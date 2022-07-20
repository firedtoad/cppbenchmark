//
// Created by Administrator on 2022/07/19.
//

#include "SortedVector.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <unordered_set>
static unsigned long
xorshf96() { /* A George Marsaglia generator, period 2^96-1 */
  static unsigned long x = 103456789, y = 362436069, z = 521088629;
  unsigned long t;

  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;

  z = t ^ x ^ y;
  return z;
}

static inline unsigned long _random() { return xorshf96(); }


template <typename V> static void BenchInsert(benchmark::State &state) {
  for (auto _ : state) {
    V v;
    for (auto i = 0; i < state.range(0); i++) {
      auto r = _random();
      v.insert(r);
    }
  }
}

BENCHMARK_TEMPLATE(BenchInsert, sorted_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchInsert, std::unordered_set<uint64_t>)
    ->Range(1, 1 << 10);

template <typename V> static void BenchFind(benchmark::State &state) {
  V v;
  for (auto i = 0; i < state.range(0); i++) {
    auto r = _random() % 65536;
    v.insert(r);
  }
  for (auto _ : state) {
    auto it = v.find(_random() % 65536);
    benchmark::DoNotOptimize(it);
  }
}

BENCHMARK_TEMPLATE(BenchFind, sorted_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchFind, std::unordered_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchRange(benchmark::State &state) {
  V v;
  for (auto i = 0; i < state.range(0); i++) {
    auto r = _random();
    v.insert(r);
  }
  auto sum = 0;
  for (auto _ : state) {
    for (auto &it : v) {
      sum += it;
    }
  }
  benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchRange, std::unordered_set<uint64_t>)->Range(1, 1 << 10);

template <typename V> static void BenchErase(benchmark::State &state) {
  V v;
  for (auto i = 0; i < state.range(0); i++) {
    auto r = _random() % 65536;
    v.insert(r);
  }
  for (auto _ : state) {
    auto r = _random() % 65536;
    auto it = v.erase(r);
    if (it > 0) {
      v.insert(r);
    }
  }
}

BENCHMARK_TEMPLATE(BenchErase, sorted_vector<uint64_t>)->Range(1, 1 << 10);
BENCHMARK_TEMPLATE(BenchErase, std::unordered_set<uint64_t>)->Range(1, 1 << 10);

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}