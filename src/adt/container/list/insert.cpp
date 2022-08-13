//
// Created by Administrator on 2022/01/16.
//

#include "linked_list.h"
#include "plf_list.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <list>
#include <llvm/ADT/simple_ilist.h>
#include <llvm/ADT/ilist.h>

using mode               = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList : public boost::intrusive::list_base_hook<mode>
{
    int x{};
};

struct LNode : public llvm::ilist_node<LNode, llvm::ilist_tag<LNode>>
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

BENCHMARK(BenchListInsert)->Range(1, 1024);


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

BENCHMARK(BenchBUListInsert)->Range(1, 1024);

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

BENCHMARK(BenchIntrusiveListInsert)->Range(1, 1024);

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

BENCHMARK(BenchPlfListInsert)->Range(1, 1024);

static void BenchAdtList(benchmark::State &state)
{

    for (auto _ : state)
    {
        llvm::ilist<LNode,llvm::ilist_tag<LNode>> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(new LNode);
        }
    }
}

BENCHMARK(BenchAdtList)->Range(1, 1024);

static void BenchAdtSimpleList(benchmark::State &state)
{

    for (auto _ : state)
    {
        llvm::simple_ilist<LNode,llvm::ilist_tag<LNode>> v;
        std::vector<LNode> vs;
        vs.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(vs[i]);
        }
    }
}

BENCHMARK(BenchAdtSimpleList)->Range(1, 1024);


int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}