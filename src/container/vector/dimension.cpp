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

#include "vector.hpp"
#include <benchmark/benchmark.h>
#include <numeric>
#include <vector>

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long random_()
{
    return xorshf96();
}

template <size_t M, size_t N> static void BenchFindDimension(benchmark::State &state)
{
    std::vector<std::vector<int>> v(M);

    for (size_t i = 0; i < M; i++)
    {
        v[i].resize(N);
    }

    for (auto _ : state)
    {
        for (auto i{0}; i < state.range(0); i++)
        {
            int x = random_() % M;
            int y = random_() % N;
            benchmark::DoNotOptimize(v[x][y]);
        }
    }
}

template <size_t M, size_t N> static void BenchFindLinear(benchmark::State &state)
{
    std::vector<int> v(M * N,0);
    for (auto _ : state)
    {
        for (auto i{0}; i < state.range(0); i++)
        {
            int x = random_() % M;
            int y = random_() % N;
            benchmark::DoNotOptimize(v[(x * M) + y]);
        }
    }
}

BENCHMARK_TEMPLATE(BenchFindDimension, 1024, 1024)->Range(1, 1024);
BENCHMARK_TEMPLATE(BenchFindLinear, 1024, 1024)->Range(1, 1024);
int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}