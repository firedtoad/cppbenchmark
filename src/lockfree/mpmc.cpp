#include "MPMCQueue.h"
#include "concurrentqueue.h"
#include <benchmark/benchmark.h>
#include <boost/lockfree/queue.hpp>
#include <cds/intrusive/vyukov_mpmc_cycle_queue.h>
#include <tbb/concurrent_queue.h>
#include <thread>

struct S
{
    char c[128]{};
};

static void Boost_Mpmc(benchmark::State &state)
{
    static boost::lockfree::queue<S, boost::lockfree::capacity<1024>> mpmc;
    for (auto _ : state)
    {
        S s;
        mpmc.push(s);
        benchmark::DoNotOptimize(mpmc.pop(s));
    }
}

BENCHMARK(Boost_Mpmc)->DenseThreadRange(1, 8);

static void MPMCQueue(benchmark::State &state)
{
    static rigtorp::MPMCQueue<S> rmpmc(100000);
    for (auto _ : state)
    {
        S s;
        rmpmc.push(s);
        benchmark::DoNotOptimize(rmpmc.try_pop(s));
    }
}

BENCHMARK(MPMCQueue)->DenseThreadRange(1, 8);

static void CCQueue(benchmark::State &state)
{
    static moodycamel::ConcurrentQueue<S> ccQueue(100000);
    for (auto _ : state)
    {
        S s;
        benchmark::DoNotOptimize(ccQueue.enqueue(s));
        benchmark::DoNotOptimize(ccQueue.try_dequeue(s));
    }
}

BENCHMARK(CCQueue)->DenseThreadRange(1, 8);

static void VYQueue(benchmark::State &state)
{
    static cds::intrusive::VyukovMPMCCycleQueue<S> vyQueue(100000);
    for (auto _ : state)
    {
        S s;
        benchmark::DoNotOptimize(vyQueue.push(s));
        benchmark::DoNotOptimize(vyQueue.pop());
    }
}

BENCHMARK(VYQueue)->DenseThreadRange(1, 8);

static void TBBQueue(benchmark::State &state)
{
    static tbb::strict_ppl::concurrent_queue<S> tbbQueue;
    for (auto _ : state)
    {
        S s;
        tbbQueue.push(s);
        benchmark::DoNotOptimize(tbbQueue.try_pop(s));
    }
}

BENCHMARK(TBBQueue)->DenseThreadRange(1, 8);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}