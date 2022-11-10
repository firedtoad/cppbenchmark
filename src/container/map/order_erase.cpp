//
// Created by Administrator on 2022/01/15.
//

#include "tsl/ordered_map.h"
#include "absl/container/btree_map.h"
#include "parallel_hashmap/btree.h"
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

template <class M> static void BenchEraseOrderMapInt(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[i] = i;
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = r;
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, std::map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, absl::btree_map<int, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapInt, phmap::btree_map<int, int>);

template <class M> static void BenchEraseOrderMapString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey ="12345678901234561234567890123456" + std::to_string(_random());
        keys[i]   = sKey;
        m[sKey]   = i;
    }
    int r{};
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = m.erase(r);
        if (it > 0)
        {
            m[r] = 0;
        }
    }
    benchmark::DoNotOptimize(r);
}


BENCHMARK_TEMPLATE(BenchEraseOrderMapString, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, tsl::ordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, absl::btree_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchEraseOrderMapString, phmap::btree_map<std::string, int>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
