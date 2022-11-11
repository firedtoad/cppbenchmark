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
#include <list>

using mode               = boost::intrusive::link_mode<boost::intrusive::auto_unlink>;
using constant_time_size = boost::intrusive::constant_time_size<false>;

#include <llvm/ADT/ilist.h>
#include <llvm/ADT/simple_ilist.h>

struct SList : public boost::intrusive::list_base_hook<mode>
{
    int x{};
};

struct LNode : public llvm::ilist_node<LNode, llvm::ilist_tag<LNode>>
{
    int x{};
};

template <typename V> void Splice(V &&v1, V &&v2)
{
    if (v1.empty())
    {
        v1.splice(v1.end(), v2);
        return;
    }
    v2.splice(v2.end(), v1);
}

static void BenchListSplice(benchmark::State &state)
{
    int r = 0;
    std::list<SList> v;
    std::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    for (auto _ : state)
    {
        Splice(v, v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSplice)->Range(1, 65536);

static void BenchListSwap(benchmark::State &state)
{
    int r = 0;
    std::list<SList> v;
    std::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    for (auto _ : state)
    {
        Splice(v, v1);
        r = v1.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSwap)->Range(1, 65536);

static void BenchIntrusiveListSplice(benchmark::State &state)
{
    boost::intrusive::list<SList, constant_time_size> vt;
    boost::intrusive::list<SList, constant_time_size> v;
    std::vector<SList> lst(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(lst[i]);
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(vt, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSplice)->Range(1, 65536);

static void BenchIntrusiveListSwap(benchmark::State &state)
{

    boost::intrusive::list<SList, constant_time_size> vt;
    boost::intrusive::list<SList, constant_time_size> v;
    std::vector<SList> lst;
    lst.resize(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(lst[i]);
    }

    int r = 0;
    for (auto _ : state)
    {
        Splice(vt, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSwap)->Range(1, 65536);

static void BenchPlfListSplice(benchmark::State &state)
{
    plf::list<SList> v;
    plf::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(v1, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListSplice)->Range(1, 65536);

static void BenchPlfListSwap(benchmark::State &state)
{
    plf::list<SList> v;
    plf::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(v1, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListSwap)->Range(1, 65536);

static void BenchAdtListSplice(benchmark::State &state)
{
    llvm::ilist<LNode, llvm::ilist_tag<LNode>> v;
    llvm::ilist<LNode, llvm::ilist_tag<LNode>> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(new LNode());
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(v1, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAdtListSplice)->Range(1, 65536);

static void BenchAdtListSwap(benchmark::State &state)
{
    llvm::ilist<LNode, llvm::ilist_tag<LNode>> v;
    llvm::ilist<LNode, llvm::ilist_tag<LNode>> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(new LNode());
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(v1, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAdtListSwap)->Range(1, 65536);

static void BenchAdtSimpleListSplice(benchmark::State &state)
{
    llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v;
    llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v1;
    std::vector<LNode> vs;
    vs.resize(state.range(0));
    v.insert(v.end(), vs.begin(), vs.end());
    int r = 0;
    for (auto _ : state)
    {
        Splice(v1, v);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAdtSimpleListSplice)->Range(1, 65536);

static void BenchAdtSimpleListSwap(benchmark::State &state)
{
    llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v;
    llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v1;
    std::vector<LNode> vs;
    vs.resize(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(vs[i]);
    }
    int r = 0;
    for (auto _ : state)
    {
        std::swap(v, v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchAdtSimpleListSwap)->Range(1, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}