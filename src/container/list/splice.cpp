//
// Created by Administrator on 2022/02/19.
//

//
// Created by Administrator on 2022/01/16.
//

#include<benchmark/benchmark.h>
#include <list>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

using mode = boost::intrusive::link_mode<boost::intrusive::auto_unlink>;
using constant_time_size = boost::intrusive::constant_time_size<false>;

struct SList : public boost::intrusive::list_base_hook<mode> {

};

static void BenchListSplice(benchmark::State &state) {

    int r = 0;
    for (auto _ : state) {
        state.PauseTiming();
        std::list<SList> v;
        std::list<SList> v1;
        for (auto i = 0; i < 1024; i++) {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.splice(v.end(), v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSplice);

static void BenchListSwap(benchmark::State &state) {

    int r = 0;
    for (auto _ : state) {
        state.PauseTiming();
        std::list<SList> v;
        std::list<SList> v1;
        for (auto i = 0; i < 1024; i++) {
            v1.push_back({});
        }
        state.ResumeTiming();
        v.swap(v1);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSwap);


static void BenchIntrusiveListSplice(benchmark::State &state) {

    int r = 0;
    for (auto _ : state) {
        state.PauseTiming();
        boost::intrusive::list<SList, constant_time_size> vt;
        boost::intrusive::list<SList, constant_time_size> v;
        SList lst[1024];
        for (auto i = 0; i < 1024; i++) {
            v.push_back(lst[i]);
        }
        state.ResumeTiming();
        vt.splice(vt.end(),v);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSplice);

static void BenchIntrusiveListSwap(benchmark::State &state) {

    int r = 0;
    for (auto _ : state) {
        state.PauseTiming();
        boost::intrusive::list<SList, constant_time_size> vt;
        boost::intrusive::list<SList, constant_time_size> v;
        SList lst[1024];
        for (auto i = 0; i < 1024; i++) {
            v.push_back(lst[i]);
        }
        state.ResumeTiming();
        vt.swap(v);
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSwap);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}