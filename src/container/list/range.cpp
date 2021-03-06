//
// Created by Administrator on 2022/01/16.
//

#include "linked_list.h"
#include "plf_list.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <deque>
#include <forward_list>
#include <list>

using mode               = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList : public boost::intrusive::list_base_hook<mode>
{
    int i;
};

static void BenchListRange(benchmark::State &state)
{
    std::list<SList> v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListRange)->Range(1, 1024);

static void BenchForwardListRange(benchmark::State &state)
{
    std::forward_list<SList> v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_front({});
    }
    int r = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchForwardListRange)->Range(1, 1024);

static void BenchDequeRange(benchmark::State &state)
{
    std::deque<SList> v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchDequeRange)->Range(1, 1024);

static void BenchVectorRange(benchmark::State &state)
{
    std::vector<SList> v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            r += it.i;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchVectorRange)->Range(1, 1024);

static void BenchBUListRange(benchmark::State &state)
{
    butil::LinkedList<SList> v;
    std::vector<butil::LinkNode<SList>> vs;
    vs.resize(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.Append(&vs[i]);
    }
    int r = 0;
    for (auto _ : state)
    {
        for (auto it = v.head(); it != v.end(); it = it->next())
        {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchBUListRange)->Range(1, 1024);

static void BenchIntrusiveListRange(benchmark::State &state)
{
    std::vector<SList> lst(state.range(0));
    boost::intrusive::list<SList, constant_time_size> v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(lst[i]);
    }
    int r = 0;
    for (auto _ : state)
    {
        for (auto &it : v)
        {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListRange)->Range(1, 1024);

static void BenchPlfListRange(benchmark::State &state)
{
    plf::list<SList> v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {

        for (auto &it : v)
        {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListRange)->Range(1, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}