//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

struct SList : public boost::intrusive::list_base_hook<> {

};

static void BenchListRange(benchmark::State &state) {
    std::list<SList> v;
    for (auto i = 0; i < 1024; i++) {
        v.push_back({});
    }
    int r = 0;
    for (auto _ : state) {
        for (auto it:v) {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListRange);


static void BenchIntrusiveListRange(benchmark::State &state) {
    boost::intrusive::list<SList> v;
    SList lists[1024];
    for (auto i = 0; i < 1024; i++) {
        v.push_back(lists[i]);
    }
    int r = 0;
    for (auto _ : state) {
        for (auto it:v) {
            r++;
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListRange);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}