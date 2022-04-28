//
// Created by Administrator on 2022/01/15.
//
#include <iostream>
#include <benchmark/benchmark.h>
#include <unordered_map>
#include <boost/container/flat_map.hpp>
#include "parallel_hashmap/phmap.h"
#include "flat_hash_map.hpp"
#include "unordered_map.hpp"
#include "bytell_hash_map.hpp"
#include "sparsepp/spp.h"
#include "tsl/sparse_map.h"
#include "tsl/array-hash/array_map.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/htrie_map.h"
#include "tsl/robin_map.h"
#include "robin_hood.h"

static unsigned long xorshf96() { /* A George Marsaglia generator, period 2^96-1 */
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

static inline unsigned long _random() {
    return xorshf96();
}

template<class M>
static void BenchUnOrderMapInt(benchmark::State &state) {
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++) {
        m[i] = i;
    }
    for (auto _ : state) {
        auto c = m.find(_random() % 65536);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderMapInt, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, boost::container::flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, ska::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, ska::bytell_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, robin_hood::unordered_flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, spp::sparse_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::bhopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::hopscotch_map<int, int>);
//BENCHMARK_TEMPLATE(BenchUnOrderMapInt,tsl::htrie_map<int,int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::robin_map<int, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapInt, tsl::sparse_map<int, int>);

template<class M>
static void BenchUnOrderMapString(benchmark::State &state) {
    M m;
    m.reserve(65536);
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++) {
        keys[i] = std::to_string(_random() % 65536);
        auto sKey = std::to_string(i);
        m[sKey] = i;
    }
    for (auto _ : state) {
        auto kIndex = _random() % 65536;
        auto c = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}
template<class M>
static void BenchCharKeyMap(benchmark::State &state) {
     M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++) {
        keys[i] = std::to_string(_random() % 65536);
        auto sKey = std::to_string(i);
        m[sKey] = i;
    }
    for (auto _ : state) {
        auto kIndex = _random() % 65536;
        auto c = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, boost::container::flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchCharKeyMap,tsl::htrie_map<char,int>);
BENCHMARK_TEMPLATE(BenchCharKeyMap,tsl::array_map<char,int>);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
