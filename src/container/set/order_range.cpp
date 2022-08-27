//
// Created by Administrator on 2022/01/15.
//

#include "tsl/ordered_set.h"
#include "absl/container/btree_set.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <set>
static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
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

static inline uint64_t _random()
{
    return xor_shift96();
}

template <class M> static void BenchRangeOrderSetInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m.insert(i);
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, std::set<int>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, tsl::ordered_set<int>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetInt, absl::btree_set<int>);

template <class M> static void BenchRangeOrderSetString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = "12345678901234561234567890123456" + std::to_string(_random());
        m.insert(sKey);
    }
    std::string r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r = it;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderSetString, std::set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetString, tsl::ordered_set<std::string>);
BENCHMARK_TEMPLATE(BenchRangeOrderSetString, absl::btree_set<std::string>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
