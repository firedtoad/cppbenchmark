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
#include <unordered_map>

#include "flat_hash_map/bytell_hash_map.hpp"
#include "flat_hash_map/flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood/robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/robin_map.h"
#include "tsl/sparse_map.h"
#include "flat_hash_map/unordered_map.hpp"
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/IndexedMap.h>
#include <llvm/ADT/IntervalMap.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/StringMap.h>
template <class M> static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_reserve, std::unordered_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, ska::unordered_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, ska::flat_hash_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, ska::bytell_hash_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, phmap::flat_hash_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, robin_hood::unordered_flat_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, spp::sparse_hash_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::bhopscotch_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::hopscotch_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::robin_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::sparse_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, llvm::DenseMap<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, llvm::MapVector<int, int>)->Range(1 << 10, 1 << 20);

template <class M> static void BM_reserveIndex(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        m.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_reserveIndex, llvm::IndexedMap<int>)->Range(1, 1 << 20);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}