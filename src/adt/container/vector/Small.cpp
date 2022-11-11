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

#include <benchmark/benchmark.h>
#include <llvm/ADT/SmallVector.h>
// #include "llvm/OPT_SmallVector.h"
// #include "llvm/vector.hpp"
#include <vector>

template <typename V> static void BenchReserve(benchmark::State &state)
{
    auto sz = state.range(0);
    for (auto _ : state)
    {
        V v{};
        v.reserve(sz);
        for (auto i = 0; i < sz; i++)
        {
            v.emplace_back(i);
        }
        benchmark::DoNotOptimize(v);
    }
}

template <typename V> static void BenchReserveSmall(benchmark::State &state)
{
    auto sz = state.range(0);

    for (auto _ : state)
    {
        V v{};
        v.reserve(sz);
        for (auto i = 0; i < sz; i++)
        {
            v.emplace_back(i);
        }
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK_TEMPLATE(BenchReserve, std::vector<uint32_t>)->Range(65536, 65536);
BENCHMARK_TEMPLATE(BenchReserveSmall, llvm::SmallVector<uint32_t, 65536>)->Range(65536, 65536);
BENCHMARK_TEMPLATE(BenchReserve, std::vector<uint64_t>)->Range(65536, 65536);
BENCHMARK_TEMPLATE(BenchReserveSmall, llvm::SmallVector<uint64_t, 65536>)->Range(65536, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}