//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
struct SList :public boost::intrusive::list_base_hook<>{

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
        SList lists[1024]{};
        boost::intrusive::list<SList> v;
        for (auto i = 0; i < 1024; i++) {
            v.push_back(lists[i]);
        }
    }
}

BENCHMARK(BenchIntrusiveListInsert);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}