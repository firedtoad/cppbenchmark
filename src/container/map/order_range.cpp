//
// Created by Administrator on 2022/01/15.
//

#include "tsl/ordered_map.h"
#include "absl/container/btree_map.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <map>
static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
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

static inline unsigned long _random()
{
    return xorshf96();
}
template <class M> static void BenchRangeOrderMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, absl::btree_map<int, int>);

template <class M> static void BenchRangeOrderMapString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]            = "12345678901234561234567890123456" +std::to_string(_random());
        m[keys[i]] = i;
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : m)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK_TEMPLATE(BenchRangeOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, absl::btree_map<std::string, int>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
