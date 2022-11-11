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

#include "absl/container/btree_set.h"
#include "tsl/ordered_set.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <llvm/ADT/ImmutableSet.h>
#include <set>

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

static void BenchRangeImmutableSetInt(benchmark::State &state)
{
    llvm::ImmutableSet<int>::Factory factory;

    std::vector<int> keys;
    keys.reserve(65536);
    auto m = factory.getEmptySet();
    for (auto i = 0; i < 65536; i++)
    {
        auto r = i;
        keys.push_back(r);
        m = factory.add(m, r);
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

BENCHMARK(BenchRangeImmutableSetInt);

template <class M> static void BenchRangeOrderSetString(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        auto sKey = std::to_string(i);
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
