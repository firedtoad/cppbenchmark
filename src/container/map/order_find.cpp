//
// Created by Administrator on 2022/01/15.
//

#include "absl/container/btree_map.h"
#include "absl/container/node_hash_map.h"
#include "tsl/ordered_map.h"
#include <boost/container/flat_map.hpp>
#include <boost/container/small_vector.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <map>

static inline uint64_t
xor_shift96() { /* A George Marsaglia generator, period 2^96-1 */
  static uint64_t x = 123456789, y = 362436069, z = 521288629;
  uint64_t t;

  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;

  z = t ^ x ^ y;
  return z;
}

static inline uint64_t _random() { return xor_shift96(); }

template <class M> static void BenchOrderMapInt(benchmark::State &state) {
  M m;
  for (auto i = 0; i < 65536; i++) {
    m[i] = i;
  }
  for (auto _ : state) {
    auto c = m.find(_random() % 65536);
    benchmark::DoNotOptimize(c);
  }
}

BENCHMARK_TEMPLATE(BenchOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(
    BenchOrderMapInt,
    tsl::ordered_map<int, int, std::hash<int>, std::equal_to<int>,
                     std::allocator<std::pair<int, int>>,
                     std::vector<std::pair<int, int>>>);
BENCHMARK_TEMPLATE(BenchOrderMapInt, absl::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchOrderMapInt,boost::container::small_flat_map<int, int,65536>);

template <class M> static void BenchOrderMapString(benchmark::State &state) {
  M m;
  std::vector<std::string> keys(65536);
  for (auto i = 0; i < 65536; i++) {
    keys[i] = std::to_string(_random() % 65536);
    auto sKey = std::to_string(i);
    m[sKey] = i;
  }
  for (auto _ : state) {
    auto kIndex = _random() % 65536;
    auto c = m.find(keys[kIndex].c_str());
    benchmark::DoNotOptimize(c);
  }
}

BENCHMARK_TEMPLATE(BenchOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString,
                   tsl::ordered_map<std::string, int, std::hash<std::string>,
                                    std::equal_to<std::string>,
                                    std::allocator<std::pair<std::string, int>>,
                                    std::vector<std::pair<std::string, int>>>);
BENCHMARK_TEMPLATE(BenchOrderMapString, absl::btree_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapString, boost::container::small_flat_map<std::string, int,1024>);
int main(int argc, char **argv) {

  tsl::ordered_map<std::string, int> m;
  std::vector<std::string> keys(128);
  for (auto i = 0; i < 128; i++) {
    keys[i] = std::to_string(_random() % 128);
    auto sKey = std::to_string(i);
    m[sKey] = i;
  }
  m.erase("125");
  for (auto &it : m) {
    std::cout <<it.first<<":"<< it.second << '\n';
  }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
  return 0;
}
