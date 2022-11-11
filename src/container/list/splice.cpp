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

#include "plf_list.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <deque>
#include <list>

using mode               = boost::intrusive::link_mode<boost::intrusive::auto_unlink>;
using constant_time_size = boost::intrusive::constant_time_size<false>;

struct SList : public boost::intrusive::list_base_hook<mode>
{
};

static void BenchListSplice(benchmark::State &state)
{
    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        std::list<SList> v;
        std::list<SList> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.splice(v.end(), v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSplice)->Range(1, 65536);

static void BenchListSwap(benchmark::State &state)
{
    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        std::list<SList> v;
        std::list<SList> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.swap(v1);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSwap)->Range(1, 65536);

static void BenchDequeSwap(benchmark::State &state)
{

    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        std::deque<SList> v;
        std::deque<SList> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.swap(v1);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchDequeSwap)->Range(1, 65536);

static void BenchIntrusiveListSplice(benchmark::State &state)
{

    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        boost::intrusive::list<SList, constant_time_size> vt;
        boost::intrusive::list<SList, constant_time_size> v;
        std::vector<SList> lst(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(lst[i]);
        }
        state.ResumeTiming();
        vt.splice(vt.end(), v);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSplice)->Range(1, 65536);

static void BenchIntrusiveListSwap(benchmark::State &state)
{

    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        boost::intrusive::list<SList, constant_time_size> vt;
        boost::intrusive::list<SList, constant_time_size> v;
        std::vector<SList> lst{static_cast<size_t>(state.range(0))};
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(lst[i]);
        }
        state.ResumeTiming();
        vt.swap(v);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSwap)->Range(1, 65536);

static void BenchPlfListSplice(benchmark::State &state)
{
    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        plf::list<SList> v;
        plf::list<SList> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.splice(v.end(), v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListSplice)->Range(1, 65536);

static void BenchPlfListSwap(benchmark::State &state)
{
    int r = 0;
    for (auto _ : state)
    {
        state.PauseTiming();
        plf::list<SList> v;
        plf::list<SList> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.swap(v1);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListSwap)->Range(1, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}