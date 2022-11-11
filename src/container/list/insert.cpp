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
#include <boost/intrusive/list_hook.hpp>
#include <deque>
#include <list>
#include <vector>

using mode               = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList : public boost::intrusive::list_base_hook<mode>
{
    int x{};
};

static void BenchListInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::list<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}

BENCHMARK(BenchListInsert)->Range(1, 65536);

static void BenchDequeInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::deque<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}

BENCHMARK(BenchDequeInsert)->Range(1, 65536);

static void BenchVectorInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<SList> v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}

BENCHMARK(BenchVectorInsert)->Range(1, 65536);

static void BenchBUListInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        butil::LinkedList<SList> v;
        std::vector<butil::LinkNode<SList>> vs;
        vs.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.Append(&vs[i]);
        }
    }
}

BENCHMARK(BenchBUListInsert)->Range(1, 65536);

static void BenchIntrusiveListInsert(benchmark::State &state)
{

    for (auto _ : state)
    {
        std::vector<SList> lst(state.range(0));
        boost::intrusive::list<SList, constant_time_size> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(lst[i]);
        }
    }
}

BENCHMARK(BenchIntrusiveListInsert)->Range(1, 65536);

static void BenchPlfListInsert(benchmark::State &state)
{

    for (auto _ : state)
    {
        plf::list<SList> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}

BENCHMARK(BenchPlfListInsert)->Range(1, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}