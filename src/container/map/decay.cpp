//
// Created by Administrator on 2022/01/15.
//

#include <benchmark/benchmark.h>
#include <unordered_map>
#include "unordered_map.hpp"
#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include <absl/container/flat_hash_map.h>

struct Hasher
{
    size_t operator()(int key) const
    {
        return 1;
    }
};

template <class M> static void BM_unmap(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (int i = 0; i < 10000; i++)
        {
            m[i] = i;
        }
    }
}
template <class M> static void BM_decay_map(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (int i = 0; i < 10000; i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_unmap, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, std::unordered_map<int, int,Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, ska::unordered_map<int, int,Hasher>);


BENCHMARK_TEMPLATE(BM_unmap, ska::flat_hash_map<int, int>);
//BENCHMARK_TEMPLATE(BM_decay_map, ska::flat_hash_map<int, int,Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, phmap::flat_hash_map<int, int,Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, absl::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, absl::flat_hash_map<int, int,Hasher>);


int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}