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
#include <vector>

const static int DIV = 1;

struct cache
{
    uint8_t c[1];
};

static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 521288629;
    uint64_t t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline uint64_t _random()
{
    return xor_shift96();
}

static void BM_cache32K(benchmark::State &state)
{
    const int N = 32 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);
    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}
static void BM_cache262K(benchmark::State &state)
{
    const int N = 262 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);
    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}

static void BM_cache4M(benchmark::State &state)
{
    const int N = 4096 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);
    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}
static void BM_cache8M(benchmark::State &state)
{
    const int N = 8192 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);
    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}
static void BM_cache16M(benchmark::State &state)
{
    const int N = 16384 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);

    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}
static void BM_cache32M(benchmark::State &state)
{
    const int N = 32768 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);
    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}

static void BM_cache64M(benchmark::State &state)
{
    const int N = 65536 * 1024 / DIV;
    std::vector<cache> cache;
    cache.resize(N);
    for (auto _ : state)
    {
        int idx      = _random() % N;
        const auto c = cache[idx];
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BM_cache32K);
BENCHMARK(BM_cache262K);
BENCHMARK(BM_cache4M);
BENCHMARK(BM_cache8M);
BENCHMARK(BM_cache16M);
BENCHMARK(BM_cache32M);
BENCHMARK(BM_cache64M);
int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}