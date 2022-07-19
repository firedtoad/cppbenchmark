//
// Created by Administrator on 2022/07/19.
//

#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>
#include <set>
#include <vector>

template <class T, class Compare = std::less<T>> struct sorted_vector {
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  iterator begin() { return V.begin(); }
  iterator end() { return V.end(); }
  const_iterator begin() const { return V.begin(); }
  const_iterator end() const { return V.end(); }
  sorted_vector(const Compare &c = Compare()) : V(), cmp(c) {}
  template <class InputIterator>
  sorted_vector(InputIterator first, InputIterator last,
                const Compare &c = Compare())
      : V(first, last), cmp(c) {
    std::sort(begin(), end(), cmp);
  }
  iterator insert(const T &t) {
    iterator it = lower_bound(begin(), end(), t, cmp);
    if (it == end() || cmp(t, *it))
      V.insert(it, t);
    return it;
  }
  const_iterator find(const T &t) const {
    const_iterator it = lower_bound(begin(), end(), t, cmp);
    return it == end() || cmp(t, *it) ? end() : it;
  }

  size_t erase(const T &t) {
    iterator it = lower_bound(begin(), end(), t, cmp);
    if (it != end()) {
      V.erase(it);
      return 1;
    }
    return 0;
  }

private:
  std::vector<T> V;
  Compare cmp;
};

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

static inline unsigned long _random() { return xorshf96(); }

template <typename V> static void BenchInsert(benchmark::State &state) {
  for (auto _ : state) {
    V v;
    for (auto i = 0; i < state.range(0); i++) {
      v.insert(i % state.range(0));
    }
  }
}
BENCHMARK_TEMPLATE(BenchInsert, sorted_vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchInsert, std::set<int>)->Range(2, 1024);

template <typename V> static void BenchFind(benchmark::State &state) {
  V v;
  for (auto i = 0; i < state.range(0); i++) {
    v.insert(i);
  }
  auto sz = state.range(0);
  for (auto _ : state) {
    auto it = v.find(_random() % sz);
    benchmark::DoNotOptimize(it);
  }
}

BENCHMARK_TEMPLATE(BenchFind, sorted_vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchFind, std::set<int>)->Range(2, 1024);

template <typename V> static void BenchRange(benchmark::State &state) {
  V v;
  for (auto i = 0; i < state.range(0); i++) {
    v.insert(i);
  }

  for (auto _ : state) {
    for (auto &it : v) {
      sum += it;
    }
  }
  benchmark::DoNotOptimize(sum);
}

BENCHMARK_TEMPLATE(BenchRange, sorted_vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchRange, std::set<int>)->Range(2, 1024);

template <typename V> static void BenchErase(benchmark::State &state) {
  V v;
  for (auto i = 0; i < state.range(0); i++) {
    v.insert(i);
  }
  auto sz = state.range(0);
  for (auto _ : state) {
    auto r = _random() % sz;
    auto it = v.erase(r);
    if (it > 0) {
      v.insert(r);
    }
  }
}

BENCHMARK_TEMPLATE(BenchErase, sorted_vector<int>)->Range(2, 1024);
BENCHMARK_TEMPLATE(BenchErase, std::set<int>)->Range(2, 1024);

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}