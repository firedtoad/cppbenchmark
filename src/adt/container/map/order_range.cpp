//
// Created by Administrator on 2022/01/15.
//

#include "absl/container/btree_map.h"
#include "tsl/ordered_map.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <llvm/ADT/ImmutableMap.h>
#include <map>

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
BENCHMARK_TEMPLATE(
    BenchRangeOrderMapInt,
    tsl::ordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<int, int>>, std::vector<std::pair<int, int>>>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, absl::btree_map<int, int>);

static void BenchRangeImmutableMapInt(benchmark::State &state)
{
    llvm::ImmutableMap<int, int>::Factory factory;

    std::vector<int> keys;
    keys.reserve(65536);
    auto imap = factory.getEmptyMap();
    for (auto i = 0; i < 65536; i++)
    {
        imap = factory.add(imap, i, i);
    }
    int r{};
    for (auto _ : state)
    {
        for (auto &it : imap)
        {
            r += it.second;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchRangeImmutableMapInt);

template <class M> static void BenchRangeOrderMapString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
        m[sKey]   = i;
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
