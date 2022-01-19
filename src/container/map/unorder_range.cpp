//
// Created by Administrator on 2022/01/15.
//
#include <iostream>
#include <benchmark/benchmark.h>
#include <unordered_map>
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


template<class M>
static void BenchRangeUnOrderMapInt(benchmark::State &state) {
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++) {
        m[i] = i;
    }
    int r{};
    for (auto _ : state) {
        for (auto it:m) {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, ska::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, ska::bytell_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, robin_hood::unordered_flat_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, spp::sparse_hash_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::bhopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::hopscotch_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::robin_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapInt, tsl::sparse_map<int, int>);

template<class M>
static void BenchRangeUnOrderMapString(benchmark::State &state) {
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++) {
        auto sKey = std::to_string(i);
        m[sKey] = i;
    }
    int r{};
    for (auto _ : state) {
        for (auto &it:m) {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

template<class M>
static void BenchRangeCharKeyMap(benchmark::State &state) {
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++) {
        auto sKey = std::to_string(i);
        m[sKey] = i;
    }
    int r{};
    for (auto _ : state) {
        for (auto &it:m) {
            r = it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, ska::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, ska::bytell_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, robin_hood::unordered_flat_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, spp::sparse_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::bhopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::hopscotch_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderMapString, tsl::sparse_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeCharKeyMap, tsl::htrie_map<char, int>);
BENCHMARK_TEMPLATE(BenchRangeCharKeyMap, tsl::array_map<char, int>);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    tsl::array_map<char, int> m;
    return 0;
}
