//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <deque>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include "plf_list.h"
#include "linked_list.h"

using mode = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList : public boost::intrusive::list_base_hook<mode> {
  int i;
};

static void BenchListRange(benchmark::State &state) {
    std::list<SList> v;
    for (auto i = 0; i < 1024; i++) {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state) {
        for (auto &it : v) {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListRange);

static void BenchDequeRange(benchmark::State &state) {
    std::deque<SList> v;
    for (auto i = 0; i < 1024; i++) {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state) {
        for (auto &it : v) {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchDequeRange);

static void BenchVectorRange(benchmark::State &state) {
  std::vector<SList> v;
  for (auto i = 0; i < 1024; i++) {
    v.push_back({});
  }
  int r = 0;
  for (auto _ : state) {
    for (auto &it : v) {
      r+=it.i;
    }
  }
  benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchVectorRange);

static void BenchBUListRange(benchmark::State &state) {
    butil::LinkedList<SList> v;
    std::vector<butil::LinkNode<SList>> vs;
    vs.resize(1024);
    for (auto i = 0; i < 1024; i++) {
        v.Append(&vs[i]);
    }
    int r = 0;
    for (auto _ : state) {
        for (auto it = v.head(); it != v.end(); it = it->next()) {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchBUListRange);

static void BenchIntrusiveListRange(benchmark::State &state) {
    SList lst[1024];
    boost::intrusive::list<SList, constant_time_size> v;
    for (auto i = 0; i < 1024; i++) {
        v.push_back(lst[i]);
    }
    int r = 0;
    for (auto _ : state) {
        for (auto &it : v) {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListRange);

static void BenchPlfListRange(benchmark::State &state) {
    plf::list<SList> v;
    for (auto i = 0; i < 1024; i++) {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state) {

        for (auto &it : v) {
            r++;
        }

    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListRange);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}