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
#include <deque>
#include <ext/pool_allocator.h>
#include <list>
#include <experimental/memory_resource>
#include <queue>
#include <string>
#include <vector>

constexpr int N = 1000;

void init(std::vector<std::string> &v1)
{
    v1.resize(N);
    for (auto i = 0; i < N; i++)
    {
        v1[i] = std::to_string(i);
    }
}

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

struct Pod
{
    char c[1024]{};
};
template <typename V> static void BenchPushPop(benchmark::State &state)
{
    V v;
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(random_());
        }
        for (; !v.empty();)
        {
            v.pop_front();
        }
    }
}

template <typename V> static void BenchQueuePushPop(benchmark::State &state)
{
    V v;
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push(random_());
        }
        for (; !v.empty();)
        {
            v.pop();
        }
    }
}

template <typename V> static void BenchPushErase(benchmark::State &state)
{
    V v{};
    v.reserve(state.range(0));
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
        v.clear();
    }
}

#if (((__GNUC__ * 100) + __GNUC_MINOR__) >= 900)
template <typename V> static void BenchPmrPushErase(benchmark::State &state)
{
    std::vector<int> buff(state.range(0));
    std::pmr::monotonic_buffer_resource pool(buff.data(), state.range(0) * sizeof(int));
    V v{&pool};
    v.reserve(state.range(0));
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
        v.clear();
    }
}

BENCHMARK_TEMPLATE(BenchPmrPushErase, std::pmr::vector<Pod>)->Range(1, 65536);
#endif

BENCHMARK_TEMPLATE(BenchPushPop, std::list<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchPushPop, std::list<int, __gnu_cxx::__pool_alloc<int>>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchQueuePushPop, std::queue<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchPushPop, std::deque<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchPushPop, std::deque<int, __gnu_cxx::__pool_alloc<int>>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchPushErase, std::vector<Pod>)->Range(1, 65536);



int main(int argc, char **argv)
{

    std::vector<int> v;
    for (auto i = 0; i < 1024; i++)
    {
        v.push_back(random_());
    }
    for (auto x : {1, 2, 3})
    {
        for (; !v.empty();)
        {
            v.pop_back();
        }
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}