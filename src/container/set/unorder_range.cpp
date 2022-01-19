//
// Created by Administrator on 2022/01/15.
//
#include <iostream>
#include <benchmark/benchmark.h>
#include <unordered_set>
#include "parallel_hashmap/phmap.h"
#include "flat_hash_map.hpp"
#include "unordered_map.hpp"
#include "bytell_hash_map.hpp"
#include "sparsepp/spp.h"
#include "tsl/sparse_set.h"
#include "tsl/array-hash/array_set.h"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include "tsl/htrie_set.h"
#include "tsl/robin_set.h"
#include "robin_hood.h"

template<class M>
static void BenchRangeUnOrderSetInt(benchmark::State &state) {
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++) {
        m.insert(i);
    }
    int r{};
    for (auto _ : state) {
        for (auto &it:m) {
            r += it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, std::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, ska::unordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, ska::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, ska::bytell_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, phmap::flat_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, robin_hood::unordered_flat_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, spp::sparse_hash_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::bhopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::hopscotch_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::robin_set<int>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetInt, tsl::sparse_set<int>);

template<class M>
static void BenchRangeUnOrderSetString(benchmark::State &state) {
    M m;
    m.reserve(65536);
    for (auto i = 0; i < 65536; i++) {
        auto sKey = std::to_string(i);
        m.insert(sKey);
    }
    std::string r{};
    for (auto _ : state) {
        for (auto &it:m) {
            r = it;
        }
    }
    benchmark::DoNotOptimize(r);
}

template<class M>
static void BenchRangeCharKeySet(benchmark::State &state) {
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++) {
        auto sKey = std::to_string(i);
        m.insert(sKey);
    }
    std::string r{};
    for (auto _ : state) {
        for (auto it = m.begin(); it != m.end(); it++) {
            r = it.key();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, std::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, ska::unordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, ska::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, ska::bytell_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, phmap::flat_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, robin_hood::unordered_flat_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, spp::sparse_hash_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::bhopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::hopscotch_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::robin_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeUnOrderSetString, tsl::sparse_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::htrie_set<char>);
BENCHMARK_TEMPLATE(BenchRangeCharKeySet, tsl::array_set<char>);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}
