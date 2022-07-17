#include <benchmark/benchmark.h>
#include <thread>
#include <boost/lockfree/spsc_queue.hpp>
#include <cds/container/weak_ringbuffer.h>
#include <cds/intrusive//vyukov_mpmc_cycle_queue.h>
#include <tbb/concurrent_queue.h>
#include "SPSCQueue.h"
#include "readerwriterqueue.h"
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/smart_ptr/detail/spinlock_std_atomic.hpp>

struct S {
    char c[128];
};



static void Boost_Spsc(benchmark::State &state) {
    static boost::lockfree::spsc_queue<S, boost::lockfree::capacity<100000>> queue;
    for (auto _ : state) {
        S s;
        queue.push(s);
        benchmark::DoNotOptimize(queue.pop(s));
    }
}

BENCHMARK(Boost_Spsc);



static void SPSCQueue(benchmark::State &state) {
    static rigtorp::SPSCQueue<S> spsc(100000);
    for (auto _ : state) {
        spsc.push({});
        benchmark::DoNotOptimize(spsc.front());
        spsc.pop();
    }
}

BENCHMARK(SPSCQueue);



static void RWQueue(benchmark::State &state) {
    static moodycamel::ReaderWriterQueue<S> rwQueue(100000);
    for (auto _ : state) {
        rwQueue.enqueue({});
        benchmark::DoNotOptimize(rwQueue.pop());
    }
}

BENCHMARK(RWQueue);



static void RBQueue(benchmark::State &state) {
    static cds::container::WeakRingBuffer<S> rbQueue(100000);
    for (auto _ : state) {
        S s;
        rbQueue.push(s);
        benchmark::DoNotOptimize(rbQueue.pop(s));
    }
}

BENCHMARK(RBQueue);

static void VYQueue(benchmark::State &state) {
    static cds::intrusive::VyukovMPMCCycleQueue<S> vyQueue(100000);
    for (auto _ : state) {
        S s;
        vyQueue.push(s);
        benchmark::DoNotOptimize(vyQueue.pop());
    }
}

BENCHMARK(VYQueue);

static void TBBQueue(benchmark::State &state) {
    static tbb::strict_ppl::concurrent_queue<S> tbbQueue;
    for (auto _ : state) {
        S s;
        tbbQueue.push({});
        benchmark::DoNotOptimize(tbbQueue.try_pop(s));
    }
}

BENCHMARK(TBBQueue);

int main(int argc, char **argv) {

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}