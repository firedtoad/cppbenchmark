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

#include "MemoryPool.h"
#include <LinearAllocator.h>
#include <PoolAllocator.h>
#include <StackAllocator.h>
#include <benchmark/benchmark.h>
#include <boost/atomic.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <ext/pool_allocator.h>
#include <foonathan/memory/memory_pool.hpp>
#include <iostream>
#include <memory_resource>
#include <thread>

struct Child
{
    uint8_t c[128];
};

static void BM_Pool(benchmark::State &state)
{
    PoolAllocator pool(sizeof(Child) * 10000, sizeof(Child));
    pool.Init();
    for (auto _ : state)
    {
        auto c = (Child *)pool.Allocate(sizeof(Child), 1);
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        pool.Free(c);
    }
}

BENCHMARK(BM_Pool);

static void BM_Pool2(benchmark::State &state)
{
    MemoryPool<Child, sizeof(Child) * 2> pool;
    for (auto _ : state)
    {
        auto c = (Child *)pool.newElement();
        pool.construct(c);
        benchmark::DoNotOptimize(c);
        pool.deleteElement(c);
    }
}

BENCHMARK(BM_Pool2);

static void BM_Resource(benchmark::State &state)
{
    std::pmr::synchronized_pool_resource pool;
    for (auto _ : state)
    {
        auto c = (Child *)pool.allocate(sizeof(Child));
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        pool.deallocate(c, sizeof(Child));
    }
}

BENCHMARK(BM_Resource);

static void BM_Monotonic(benchmark::State &state)
{
    char buff[8192];
    std::pmr::monotonic_buffer_resource pool(buff, std::size(buff));
    for (auto _ : state)
    {
        auto c = (Child *)pool.allocate(sizeof(Child));
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        pool.deallocate(c, sizeof(Child));
    }
}

BENCHMARK(BM_Monotonic);

static void BM_GCC(benchmark::State &state)
{

    ::__gnu_cxx::__pool_alloc<Child> pool;
    for (auto _ : state)
    {
        auto c = pool.allocate(1);
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        pool.deallocate(c, 1);
    }
}

BENCHMARK(BM_GCC);

static void BM_Boost(benchmark::State &state)
{
    boost::pool<> pool(sizeof(Child));
    for (auto _ : state)
    {
        auto c = (Child *)pool.malloc();
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        pool.free(c);
    }
}

BENCHMARK(BM_Boost);

static void BM_Foo(benchmark::State &state)
{
    foonathan::memory::memory_pool<> pool(sizeof(Child), sizeof(Child) * 2);
    for (auto _ : state)
    {
        auto c = (Child *)pool.allocate_node();
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        pool.deallocate_node(c);
    }
}

BENCHMARK(BM_Foo);

static void BM_lockless(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto c = (Child *)malloc(sizeof(Child));
        new (c) Child();
        benchmark::DoNotOptimize(c);
        c->~Child();
        free(c);
    }
}

BENCHMARK(BM_lockless);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}