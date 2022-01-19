//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

struct SList : public boost::intrusive::list_base_hook<> {

};

static void BenchListRemove(benchmark::State &state) {
    std::list<SList> v;

    for (auto _ : state) {
        for (auto i = 0; i < 1024; i++) {
            v.push_back({});
        }
        while (v.size()) {
            v.pop_front();
        }
    }
}

BENCHMARK(BenchListRemove);


static void BenchIntrusiveListRemove(benchmark::State &state) {
    boost::intrusive::list<SList> v;
    for (auto _ : state) {
        SList lists[1024];
        for (auto i = 0; i < 1024; i++) {
            v.push_back(lists[i]);
        }
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