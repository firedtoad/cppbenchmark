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

#include "linked_list.h"
#include "plf_list.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <vector>

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 103456789, y = 362436069, z = 521088629;
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

static inline unsigned long _random()
{
    return xorshf96();
}

using mode               = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList : public boost::intrusive::list_base_hook<mode>
{
};

static void BenchListRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::list<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto it = std::next(v.begin(), _random() % v.size());
            v.erase(it);
        }
    }
}

BENCHMARK(BenchListRemove)->Range(1, 65536);

static void BenchForwardListRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::forward_list<SList> v;
        auto sz = state.range(0);
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_front({});
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto idx      = sz > 1 ? _random() % --sz : 0;
            const auto it = std::next(v.begin(), idx);
            if (idx == 0)
            {
                v.erase_after(v.before_begin());
            }
            else
            {
                v.erase_after(it);
            }
        }
    }
}

BENCHMARK(BenchForwardListRemove)->Range(1, 65536);

static void BenchDequeRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::deque<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto it = std::next(v.begin(), _random() % v.size());
            v.erase(it);
        }
    }
}

BENCHMARK(BenchDequeRemove)->Range(1, 65536);

static void BenchVectorRemove(benchmark::State &state)
{

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto it = std::next(v.begin(), _random() % v.size());
            v.erase(it);
        }
    }
}

BENCHMARK(BenchVectorRemove)->Range(1, 65536);

static void BenchBUListRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        butil::LinkedList<SList> v;
        std::vector<butil::LinkNode<SList>> vs;
        vs.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.Append(&vs[i]);
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v);
            v.head()->RemoveFromList();
        }
    }
}

BENCHMARK(BenchBUListRemove)->Range(1, 65536);

static void BenchIntrusiveListRemove(benchmark::State &state)
{

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<SList> lst(state.range(0));
        boost::intrusive::list<SList, constant_time_size> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(lst[i]);
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto it = std::next(v.begin(), _random() % v.size());
            v.erase(it);
        }
    }
}

BENCHMARK(BenchIntrusiveListRemove)->Range(1, 65536);

static void BenchPlfListRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        plf::list<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
        state.ResumeTiming();
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto it = std::next(v.begin(), _random() % v.size());
            v.erase(it);
        }
    }
}

BENCHMARK(BenchPlfListRemove)->Range(1, 65536);

int main(int argc, char **argv)
{

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    for (auto it = v.begin(); it != v.end();)
    {
        if (*it < 6)
        {
            it = v.erase(it);
            continue;
        }
        it++;
    }
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ""));

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}