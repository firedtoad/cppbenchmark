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
#include <absl/container/flat_hash_map.h>
#include <benchmark/benchmark.h>
#include <unordered_map>

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
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}