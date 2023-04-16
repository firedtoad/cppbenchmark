// Copyright 2020 The Division Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Author dietoad@gmail.com && firedtoad@gmail.com

#include "SPSCQueue.h"
#include "readerwriterqueue.h"
#include <benchmark/benchmark.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <cds/container/weak_ringbuffer.h>
#include <cds/intrusive/vyukov_mpmc_cycle_queue.h>
#include <tbb/concurrent_queue.h>
#include <thread>

struct S
{
    char c[128];
};

static void Boost_Spsc(benchmark::State &state)
{
    static boost::lockfree::spsc_queue<S, boost::lockfree::capacity<100000>> queue;
    for (auto _ : state)
    {
        S s;
        queue.push(s);
        benchmark::DoNotOptimize(queue.pop(s));
    }
}

BENCHMARK(Boost_Spsc);

static void SPSCQueue(benchmark::State &state)
{
    static rigtorp::SPSCQueue<S> spsc(100000);
    for (auto _ : state)
    {
        spsc.push({});
        benchmark::DoNotOptimize(spsc.front());
        spsc.pop();
    }
}

BENCHMARK(SPSCQueue);

static void RWQueue(benchmark::State &state)
{
    static moodycamel::ReaderWriterQueue<S> rwQueue(100000);
    for (auto _ : state)
    {
        rwQueue.enqueue({});
        benchmark::DoNotOptimize(rwQueue.pop());
    }
}

BENCHMARK(RWQueue);

static void RBQueue(benchmark::State &state)
{
    static cds::container::WeakRingBuffer<S> rbQueue(100000);
    for (auto _ : state)
    {
        S s;
        rbQueue.push(s);
        benchmark::DoNotOptimize(rbQueue.pop(s));
    }
}

BENCHMARK(RBQueue);

static void VYQueue(benchmark::State &state)
{
    static cds::intrusive::VyukovMPMCCycleQueue<S> vyQueue(100000);
    for (auto _ : state)
    {
        S s;
        vyQueue.push(s);
        benchmark::DoNotOptimize(vyQueue.pop());
    }
}

BENCHMARK(VYQueue);

static void TBBQueue(benchmark::State &state)
{
    static tbb::concurrent_bounded_queue<S> tbbQueue;
    tbbQueue.set_capacity(100000);
    for (auto _ : state)
    {
        S s;
        tbbQueue.push({});
        benchmark::DoNotOptimize(tbbQueue.try_pop(s));
    }
}

BENCHMARK(TBBQueue);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}