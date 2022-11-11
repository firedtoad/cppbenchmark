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

static void BM_SortUnique(benchmark::State &state)
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
        std::sort(vec.begin(), vec.end());
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
        std::sort(vec.begin(), vec.end());
    }
}

BENCHMARK(BM_SortUnique)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor, std::set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor, std::unordered_set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueManually, std::set<int>)->Range(128, 1 << 20);
BENCHMARK_TEMPLATE(BM_UniqueManually, std::unordered_set<int>)->Range(128, 1 << 20);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}