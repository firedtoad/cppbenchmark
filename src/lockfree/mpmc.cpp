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

#include "MPMCQueue.h"
#include "concurrentqueue.h"
#include "mpmc_queue.hpp"
#include <benchmark/benchmark.h>
#include <boost/lockfree/queue.hpp>
#include <cds/intrusive/vyukov_mpmc_cycle_queue.h>
#include <tbb/concurrent_queue.h>

struct S
{
    char c[128]{};
};

static void Boost_LockFree_queue(benchmark::State &state)
{
    static boost::lockfree::queue<S, boost::lockfree::capacity<65534>> mpmc;
    for (auto _ : state)
    {
        S s;
        mpmc.push(s);
        auto q=mpmc.pop(s);
        benchmark::DoNotOptimize(q);
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(Boost_LockFree_queue)->DenseThreadRange(1, 8);

static void Rigtorp_MPMCQueue(benchmark::State &state)
{
    static rigtorp::MPMCQueue<S> rmpmc(1<<20);
    for (auto _ : state)
    {
        S s;
        rmpmc.push(s);
        auto q=rmpmc.try_pop(s);
        benchmark::DoNotOptimize(q);
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(Rigtorp_MPMCQueue)->DenseThreadRange(1, 8);

static void _1024Cores_MPMCQueue(benchmark::State &state)
{
    static mpmc_bounded_queue<S> mpmc(1<<20);
    for (auto _ : state)
    {
        S s;
        mpmc.enqueue(s);
        auto q=mpmc.dequeue(s);
        benchmark::DoNotOptimize(q);
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(_1024Cores_MPMCQueue)->DenseThreadRange(1, 8);

static void moodycamel_ConcurrentQueue(benchmark::State &state)
{
    static moodycamel::ConcurrentQueue<S> ccQueue(1<<20);
    for (auto _ : state)
    {
        S s;
        ccQueue.enqueue(s);
        auto b = ccQueue.try_dequeue(s);
        benchmark::DoNotOptimize(b);
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(moodycamel_ConcurrentQueue)->DenseThreadRange(1, 8);

static void cds_VYQueue(benchmark::State &state)
{
    static cds::intrusive::VyukovMPMCCycleQueue<S> vyQueue(1<<20);
    for (auto _ : state)
    {
        S s;
        vyQueue.push(s);
        auto q = vyQueue.pop();
        benchmark::DoNotOptimize(q);
    }
}

BENCHMARK(cds_VYQueue)->DenseThreadRange(1, 8);

static void TBBQueue(benchmark::State &state)
{
    static tbb::concurrent_bounded_queue<S> tbbQueue;
    tbbQueue.set_capacity(1<<20);
    for (auto _ : state)
    {
        S s;
        tbbQueue.push(s);
        auto q = tbbQueue.try_pop(s);
        benchmark::DoNotOptimize(q);
    }
}

BENCHMARK(TBBQueue)->DenseThreadRange(1, 8);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}