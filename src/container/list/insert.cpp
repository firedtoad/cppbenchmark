//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

using mode = boost::intrusive::link_mode<boost::intrusive::safe_link>;
using constant_time_size = boost::intrusive::constant_time_size<true>;

struct SList :public boost::intrusive::list_base_hook<mode>{

};
static void BenchListInsert(benchmark::State &state) {
    for (auto _ : state) {
        std::list<int> v;
        for (auto i = 0; i < 1024; i++) {
            v.push_back({});
        }
    }
}

BENCHMARK(BenchListInsert);

static void BenchIntrusiveListInsert(benchmark::State &state) {

    for (auto _ : state) {
        boost::intrusive::list<SList,constant_time_size> v;
        SList lst[1024];
        for (auto i = 0; i < 1024; i++) {
          v.push_back(lst[i]);
        }
    }
}

BENCHMARK(BenchIntrusiveListInsert);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}