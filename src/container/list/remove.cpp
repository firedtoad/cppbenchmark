//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <boost/intrusive/list.hpp>

struct SList : public boost::intrusive::list_base_hook<> {

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
            v.pop_front();
        }
    }
}

BENCHMARK(BenchListRemove);


static void BenchIntrusiveListRemove(benchmark::State &state) {

    for (auto _ : state) {
        state.PauseTiming();
        SList lists[1024];
        boost::intrusive::list<SList> v;

        for (auto i = 0; i < 1024; i++) {
            v.push_back(lists[i]);
        }
        state.ResumeTiming();
        while (v.size()) {
            v.pop_front();
        }
    }
}

BENCHMARK(BenchIntrusiveListRemove);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}