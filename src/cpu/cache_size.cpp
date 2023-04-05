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
#include <numeric>
#include <random>
#include <vector>
struct CacheLine
{
    uint64_t c[8]{};
};


static void BM_CacheLine(benchmark::State &state)
{
    int count     = state.range(0) / 64;
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

BENCHMARK(BM_CacheLine)->DenseRange(32 * 1024, 1024 * 1024, 256 * 1024);
BENCHMARK(BM_CacheLine)->DenseRange(2 * 1024 * 1024, 64 * 1024 * 1024, 2 * 1024 * 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}