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
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

static void BM_AllocSize(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto p = malloc(state.range(0));
        free(p);
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_AllocSize)->Range(1, 1 << 20);

int CallFunction(const std::function<void()> &fun)
{
    fun();
    return 0;
}

template <class F> int CallLambda(F &&fun)
{
    fun();
    return 0;
}

static void BM_FunctionStack(benchmark::State &state)
{
    int *p = new int;
    std::unordered_map<int, int> umap;
    for (auto _ : state)
    {
        auto ret = CallFunction(
            [p, &umap]
            {
                benchmark::DoNotOptimize(p);
                benchmark::DoNotOptimize(umap);
            });
        benchmark::DoNotOptimize(ret);
    }
    delete p;
}
BENCHMARK(BM_FunctionStack);

static void BM_FunctionHeap(benchmark::State &state)
{
    int *p  = new int;
    int *p1 = new int;
    std::unordered_map<int, int> umap;
    for (auto _ : state)
    {
        auto ret = CallFunction(
            [p, p1, &umap]
            {
                benchmark::DoNotOptimize(p);
                benchmark::DoNotOptimize(p1);
                benchmark::DoNotOptimize(umap);
            });
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_FunctionHeap);

static void BM_Lambda(benchmark::State &state)
{
    int *p  = new int;
    int *p1 = new int;
    std::unordered_map<int, int> umap;

    for (auto _ : state)
    {
        auto ret = CallLambda(
            [p, p1, &umap]
            {
                benchmark::DoNotOptimize(umap);
                benchmark::DoNotOptimize(p);
                benchmark::DoNotOptimize(p1);
            });
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_Lambda);

int main(int argc, char **argv)
{
    std::cout << std::_Function_base::_M_max_size << '\n';
    std::cout << std::_Function_base::_M_max_align << '\n';
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    std::map<int, int> m1;
    std::map<int, int> m2;
    if (m1 == m2)
    {
    }
    return 0;
}