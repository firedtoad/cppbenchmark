//
// Created by Administrator on 2021/9/23.
//

#include <benchmark/benchmark.h>
#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>

constexpr int N = 1000;

void init(std::vector<std::string> &v1) {
  v1.resize(N);
  for (auto i = 0; i < N; i++) {
    v1[i] = std::to_string(i);
  }
}

static unsigned long
xorshf96() { /* A George Marsaglia generator, period 2^96-1 */
  static unsigned long x = 123456789, y = 362436069, z = 521288629;
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

static inline unsigned long random_() { return xorshf96(); }
template <typename V> static void BenchPushPop(benchmark::State &state) {
  V v;
  for (auto _ : state) {
    for (auto i = 0; i < state.range(0); i++) {
      v.push_back(random_());
    }
    for (; !v.empty();) {
      v.pop_front();
    }
  }
}

template <typename V> static void BenchQueuePushPop(benchmark::State &state) {
  V v;
  for (auto _ : state) {
    for (auto i = 0; i < state.range(0); i++) {
      v.push(random_());
    }
    for (; !v.empty();) {
      v.pop();
    }
  }
}

template <typename V> static void BenchPushErase(benchmark::State &state) {
  V v;
  for (auto _ : state) {
    for (auto i = 0; i < state.range(0); i++) {
      v.push_back(random_());
    }
    for (; !v.empty();) {
      v.pop_back();
    }
  }
}

BENCHMARK_TEMPLATE(BenchPushPop, std::list<int>)->Range(1, 1024);
BENCHMARK_TEMPLATE(BenchQueuePushPop, std::queue<int>)->Range(1, 1024);
BENCHMARK_TEMPLATE(BenchPushPop, std::deque<int>)->Range(1, 1024);
BENCHMARK_TEMPLATE(BenchPushErase, std::vector<int>)->Range(1, 1024);

int main(int argc, char **argv) {

  std::vector<int> v;
  for (auto i = 0; i < 1024; i++) {
    v.push_back(random_());
  }
  for (auto x : {1, 2, 3}) {
    for (; !v.empty();) {
      v.pop_back();
    }
  }
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}