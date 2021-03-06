//
// Created by Administrator on 2022/01/15.
//

#include "tsl/ordered_set.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <set>

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
template <class S> static void BenchEraseOrderSetInt(benchmark::State &state)
{
    S s;
    for (auto i = 0; i < 65536; i++)
    {
        s.insert(i);
    }
    for (auto _ : state)
    {
        auto r  = _random() % 65536;
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, std::set<int>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetInt, tsl::ordered_set<int>);

template <class S> static void BenchEraseOrderSetString(benchmark::State &state)
{
    S s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        keys[i]   = sKey;
        s.insert(sKey);
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = s.erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseOrderSetString, std::set<std::string>);
BENCHMARK_TEMPLATE(BenchEraseOrderSetString, tsl::ordered_set<std::string>);
template <class S> static void BenchEraseUnorderSetString(benchmark::State &state)
{
    S s;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        keys[i]   = sKey;
        s.insert(sKey);
    }
    for (auto _ : state)
    {
        auto r  = keys[_random() % 65536];
        auto it = s.unordered_erase(r);
        if (it > 0)
        {
            s.insert(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEraseUnorderSetString, tsl::ordered_set<std::string>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
