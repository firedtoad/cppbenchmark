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
#include <set>
#include <unordered_set>
#include <vector>
#include <list>
#include <iostream>
#include <variant>
#include "tsl/ordered_set.h"
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

static void BM_SortUniqueList(benchmark::State &state)
{
    std::list<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec = vi;
        vec.sort();
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    }
}

static void BM_SortUniqueVector(benchmark::State &state)
{
    std::vector<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec = vi;
        std::sort(vec.begin(), vec.end());
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    }
}

template <class S> static void BM_UniqueConstructor(benchmark::State &state)
{
    std::vector<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec = vi;
        S s(vec.begin(), vec.end());
        vec.assign(s.begin(), s.end());
//        std::sort(vec.begin(), vec.end());
    }
}

template <class S> static void BM_UniqueManually(benchmark::State &state)
{
    std::vector<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec = vi;
        S s;
        for (int i : vec)
            s.insert(i);
        vec.assign(s.begin(), s.end());
//        std::sort(vec.begin(), vec.end());
    }
}

BENCHMARK(BM_SortUniqueList)->Range(128, 1 << 20);
BENCHMARK(BM_SortUniqueVector)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor, std::set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor, std::unordered_set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor, tsl::vector_set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueManually, std::set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueManually, std::unordered_set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueManually, tsl::vector_set<int>)->Range(128, 1 << 20);

int main(int argc, char **argv)
{
    std::variant<int64_t,std::string> s;
    s="aaa";
    std::cout<<std::get<1>(s)<<'\n';
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}