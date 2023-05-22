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
#include <stdint.h>

__attribute__((noinline)) float calc_float(float x, float y)
{
    return x * y;
}

__attribute__((noinline)) double calc_double(double x, double y)
{
    return x * y;
}

__attribute__((noinline)) int calc_32(int x, int y)
{
    return x * y;
}

__attribute__((noinline)) int64_t calc_64(int64_t x, int64_t y)
{
    return x * y;
}

__attribute__((noinline)) int64_t calc_64d(int64_t x, double y)
{
    return x * y;
}

static void calc_int(benchmark::State &state)
{
    int d = 1;
    for (auto _ : state)
    {
        auto r=calc_32(d + 100000, d);
        benchmark::DoNotOptimize(r);
        d++;
    }
}

BENCHMARK(calc_int);

static void calc_int64(benchmark::State &state)
{
    int64_t d = 1;
    for (auto _ : state)
    {
        auto r=calc_64(d + 100000, d);
        benchmark::DoNotOptimize(r);
        d++;
    }
}

BENCHMARK(calc_int64);

static void calc_int64d(benchmark::State &state)
{
    int64_t d = 1;
    for (auto _ : state)
    {
        auto r=calc_64d(d + 100000, d);
        benchmark::DoNotOptimize(r);
        d++;
    }
}

BENCHMARK(calc_int64d);

static void calc_float(benchmark::State &state)
{
    float d = 1;
    for (auto _ : state)
    {
        auto r=calc_float(d + 100000, d);
        benchmark::DoNotOptimize(r);
        d++;
    }
}

BENCHMARK(calc_float);

static void calc_double(benchmark::State &state)
{
    double d = 1;
    for (auto _ : state)
    {
        auto r=calc_double(d + 100000, d);
        benchmark::DoNotOptimize(r);
        d++;
    }
}

BENCHMARK(calc_double);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}