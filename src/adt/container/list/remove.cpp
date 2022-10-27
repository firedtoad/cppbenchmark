//
// Created by Administrator on 2022/01/16.
//

#include "linked_list.h"
#include "plf_list.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <forward_list>
#include <iostream>
#include <list>
#include <llvm/ADT/ilist.h>
#include <llvm/ADT/simple_ilist.h>

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
    int x;
};

struct LNode : public llvm::ilist_node<LNode, llvm::ilist_tag<LNode>>
{
    int x{};
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
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_front({});
        }
        state.ResumeTiming();
        auto size = state.range(0);
        while (!v.empty())
        {
            benchmark::DoNotOptimize(v.front());
            auto it = std::next(v.begin(), _random() % size);
            benchmark::DoNotOptimize(it);
            v.pop_front();
            size--;
        }
    }
}

BENCHMARK(BenchForwardListRemove)->Range(1, 65536);

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
            benchmark::DoNotOptimize(v.head());
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

static void BenchAdtListRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        llvm::ilist<LNode, llvm::ilist_tag<LNode>> v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(new LNode());
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

BENCHMARK(BenchAdtListRemove)->Range(1, 65536);

static void BenchAdtSimpleListRemove(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        llvm::simple_ilist<LNode, llvm::ilist_tag<LNode>> v;
        std::vector<LNode> vs;
        vs.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back(vs[i]);
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

BENCHMARK(BenchAdtSimpleListRemove)->Range(1, 65536);

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