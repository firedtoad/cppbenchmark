//
// Created by Administrator on 2022/01/16.
//

#include "linked_list.h"
#include "plf_list.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <vector>

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

using mode = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList : public boost::intrusive::list_base_hook<mode> {};

static void BenchListRemove(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::list<SList> v;
    for (auto i = 0; i < 1024; i++) {
      v.push_back({});
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.front());
      auto it = std::next(v.begin(), _random() % v.size());
      v.erase(it);
    }
  }
}

BENCHMARK(BenchListRemove);
static void BenchForwardListRemove(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::forward_list<SList> v;
    for (auto i = 0; i < 1024; i++) {
      v.push_front({});
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.front());
      v.pop_front();
    }
  }
}

BENCHMARK(BenchForwardListRemove);

static void BenchDequeRemove(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::deque<SList> v;
    for (auto i = 0; i < 1024; i++) {
      v.push_back({});
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.front());
      auto it = std::next(v.begin(), _random() % v.size());
      v.erase(it);
    }
  }
}

BENCHMARK(BenchDequeRemove);

static void BenchVectorRemove(benchmark::State &state) {

  for (auto _ : state) {
    state.PauseTiming();
    std::vector<SList> v;
    for (auto i = 0; i < 1024; i++) {
      v.push_back({});
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.front());
      auto it = std::next(v.begin(), _random() % v.size());
      v.erase(it);
    }
  }
}

BENCHMARK(BenchVectorRemove);

static void BenchBUListRemove(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    butil::LinkedList<SList> v;
    std::vector<butil::LinkNode<SList>> vs;
    vs.resize(1024);
    for (auto i = 0; i < 1024; i++) {
      v.Append(&vs[i]);
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.head());
      v.head()->RemoveFromList();
    }
  }
}

BENCHMARK(BenchBUListRemove);

static void BenchIntrusiveListRemove(benchmark::State &state) {

  for (auto _ : state) {
    state.PauseTiming();
    SList lst[1024];
    boost::intrusive::list<SList, constant_time_size> v;
    for (auto i = 0; i < 1024; i++) {
      v.push_back(lst[i]);
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.front());
      auto it = std::next(v.begin(), _random() % v.size());
      v.erase(it);
    }
  }
}

BENCHMARK(BenchIntrusiveListRemove);

static void BenchPlfListRemove(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    plf::list<SList> v;
    for (auto i = 0; i < 1024; i++) {
      v.push_back({});
    }
    state.ResumeTiming();
    while (!v.empty()) {
      benchmark::DoNotOptimize(v.front());
      auto it = std::next(v.begin(), _random() % v.size());
      v.erase(it);
    }
  }
}

BENCHMARK(BenchPlfListRemove);

int main(int argc, char **argv) {

  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  for (auto it = v.begin(); it != v.end();) {
    if (*it < 6) {
      v.erase(it++);
      continue;
    }
    it++;
  }
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ""));

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}