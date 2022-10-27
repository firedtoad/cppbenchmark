//
// Created by Administrator on 2022/02/19.
//

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
        std::vector<SList> lst;
        lst.resize(state.range(0));
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

static void BenchAdtListSplice(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        llvm::ilist<LNode, llvm::ilist_tag<LNode>> v;
        llvm::ilist<LNode, llvm::ilist_tag<LNode>> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(new LNode());
        }
        state.ResumeTiming();
        v1.splice(v1.end(), v);
    }
}

BENCHMARK(BenchAdtListSplice)->Range(1, 65536);

static void BenchAdtListSwap(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        llvm::ilist<LNode, llvm::ilist_tag<LNode>> v;
        llvm::ilist<LNode, llvm::ilist_tag<LNode>> v1;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(new LNode());
        }
        state.ResumeTiming();
        v1.swap(v);
    }
}

BENCHMARK(BenchAdtListSwap)->Range(1, 65536);

static void BenchAdtSimpleListSplice(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v;
        llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v1;
        std::vector<LNode> vs;
        vs.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(vs[i]);
        }
        state.ResumeTiming();
        v1.splice(v1.end(), v);
    }
}

BENCHMARK(BenchAdtSimpleListSplice)->Range(1, 65536);

static void BenchAdtSimpleListSwap(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v;
        llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v1;
        std::vector<LNode> vs;
        vs.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(vs[i]);
        }
        state.ResumeTiming();
        v1.swap(v);
    }
}

BENCHMARK(BenchAdtSimpleListSwap)->Range(1, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}