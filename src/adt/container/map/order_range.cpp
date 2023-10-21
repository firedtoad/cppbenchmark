// Copyright 2020 The Division Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Author dietoad@gmail.com && firedtoad@gmail.com

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
BENCHMARK_TEMPLATE(BenchRangeOrderMapInt, tsl::vector_map<int, int>);
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
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, tsl::vector_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchRangeOrderMapString, absl::btree_map<std::string, int>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
