//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <deque>
#include <boost/intrusive/list.hpp>
#include "plf_list.h"
#include "linked_list.h"

using mode = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList :public boost::intrusive::list_base_hook<mode>{

};

static void BenchListRemove(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::list<SList> v;
        for (auto i = 0; i < 1024; i++) {
            v.push_back({});
        }
        state.ResumeTiming();
        while (v.size()) {
          benchmark::DoNotOptimize(v.front());
          v.pop_front();
        }
    }
}

BENCHMARK(BenchListRemove);

static void BenchDequeRemove(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::deque<SList> v;
        for (auto i = 0; i < 1024; i++) {
            v.push_back({});
        }
        state.ResumeTiming();
        while (v.size()) {
            benchmark::DoNotOptimize(v.front());
            v.pop_front();
        }
    }
}

BENCHMARK(BenchDequeRemove);


static void BenchBUListRemove(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        butil::LinkedList<SList> v;
        std::vector<butil::LinkNode<SList>> vs;
        vs.resize(1024);
        for (auto i = 0; i < 1024; i++) {
            v.Append(&vs[i]);
        }
        state.ResumeTiming();
        while (!v.empty()) {
            benchmark::DoNotOptimize(v.head());
            v.head()->RemoveFromList();
        }
    }
}

BENCHMARK(BenchBUListRemove);


static void BenchIntrusiveListRemove(benchmark::State &state) {

    for (auto _ : state) {
        state.PauseTiming();
        SList lst[1024];
        boost::intrusive::list<SList,constant_time_size> v;
        for (auto i = 0; i < 1024; i++) {
          v.push_back(lst[i]);
        }
        state.ResumeTiming();
        while (v.size()) {
          benchmark::DoNotOptimize(v.front());
          v.pop_front();
        }
    }
}

BENCHMARK(BenchIntrusiveListRemove);

static void BenchPlfListRemove(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        plf::list<SList> v;
        for (auto i = 0; i < 1024; i++) {
            v.push_back({});
        }
        state.ResumeTiming();
        while (v.size()) {
            benchmark::DoNotOptimize(v.front());
            v.pop_front();
        }
    }
}

BENCHMARK(BenchPlfListRemove);


int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}