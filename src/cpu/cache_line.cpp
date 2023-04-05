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
#include <iostream>
#include <numeric>
#include <random>
#include <unistd.h>
#include <vector>

struct CacheLine
{
    uint64_t c[8]{};
};
template <size_t N> struct S
{
    size_t size = N;
};
template <typename T> static void BM_CacheSpeed(benchmark::State &state)
{
    T t;
    const static int DIV = 64;
    int count            = t.size / DIV;
    std::vector<CacheLine> cache(count);
    std::vector<uint64_t> indices(count);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::shuffle(indices.begin(), indices.end(), rd);
    size_t i = 0;
    for (auto _ : state)
    {
        int idx = indices[i];
        auto c  = cache[idx];
        benchmark::DoNotOptimize(c);
        benchmark::DoNotOptimize(idx);
        i = i < indices.size() - 1 ? ++i : 0;
    }
}

BENCHMARK_TEMPLATE(BM_CacheSpeed, S<32 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<256 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<1024 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<2 * 1024 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<4 * 1024 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<8 * 1024 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<16 * 1024 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<32 * 1024 * 1024>);
BENCHMARK_TEMPLATE(BM_CacheSpeed, S<64 * 1024 * 1024>);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}