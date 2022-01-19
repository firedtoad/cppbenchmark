//
// Created by Administrator on 2022/01/15.
//

#include <iostream>
#include <benchmark/benchmark.h>
#include <map>
#include "tsl/ordered_map.h"

static inline uint64_t xor_shift96() { /* A George Marsaglia generator, period 2^96-1 */
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

static inline uint64_t _random() {
    return xor_shift96();
}


template<class M>
static void BenchOrderMapInt(benchmark::State &state) {
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

template<class M>
static void BenchOrderMapString(benchmark::State &state) {
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


int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
