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

#include "bytell_hash_map.hpp"
#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "robin_hood.h"
#include "sparsepp/spp.h"
#include "tsl/bhopscotch_map.h"
#include "tsl/hopscotch_map.h"
#include "tsl/robin_map.h"
#include "tsl/sparse_map.h"
#include "unordered_map.hpp"
#include "utils/symbol.h"
#include <absl/container/flat_hash_map.h>
#include <benchmark/benchmark.h>
#include <unordered_map>
#include <utils/rss.h>

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
BENCHMARK_TEMPLATE(BM_reserve, absl::flat_hash_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, robin_hood::unordered_flat_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, spp::sparse_hash_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::bhopscotch_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::hopscotch_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::robin_map<int, int>)->Range(1 << 10, 1 << 20);
BENCHMARK_TEMPLATE(BM_reserve, tsl::sparse_map<int, int>)->Range(1 << 10, 1 << 20);



int main(int argc, char **argv)
{

    BM_Memory<std::unordered_map<int, int>, 1 << 20>();
    BM_Memory<ska::unordered_map<int, int>, 1 << 20>();
    BM_Memory<ska::flat_hash_map<int, int>, 1 << 20>();
    BM_Memory<ska::bytell_hash_map<int, int>, 1 << 20>();
    BM_Memory<phmap::flat_hash_map<int, int>, 1 << 20>();
    BM_Memory<absl::flat_hash_map<int, int>, 1 << 20>();
    BM_Memory<robin_hood::unordered_flat_map<int, int>, 1 << 20>();
    BM_Memory<spp::sparse_hash_map<int, int>, 1 << 20>();
    BM_Memory<tsl::bhopscotch_map<int, int>, 1 << 20>();
    BM_Memory<tsl::hopscotch_map<int, int>, 1 << 20>();
    BM_Memory<tsl::robin_map<int, int>, 1 << 20>();
    BM_Memory<tsl::sparse_map<int, int>, 1 << 20>();

    BM_MemoryString<std::unordered_map<std::string, int>, 1 << 20>();
    BM_MemoryString<ska::unordered_map<std::string, int>, 1 << 20>();
    BM_MemoryString<ska::flat_hash_map<std::string, int>, 1 << 20>();
    BM_MemoryString<ska::bytell_hash_map<std::string, int>, 1 << 20>();
    BM_MemoryString<phmap::flat_hash_map<std::string, int>, 1 << 20>();
    BM_MemoryString<absl::flat_hash_map<std::string, int>, 1 << 20>();
    BM_MemoryString<robin_hood::unordered_flat_map<std::string, int>, 1 << 20>();
    BM_MemoryString<spp::sparse_hash_map<std::string, int>, 1 << 20>();
    BM_MemoryString<tsl::bhopscotch_map<std::string, int>, 1 << 20>();
    BM_MemoryString<tsl::hopscotch_map<std::string, int>, 1 << 20>();
    BM_MemoryString<tsl::robin_map<std::string, int>, 1 << 20>();
    BM_MemoryString<tsl::sparse_map<std::string, int>, 1 << 20>();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}