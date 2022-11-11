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

// #define BOOST_CONTEXT_USE_MAP_STACK
#include <benchmark/benchmark.h>
#include <boost/context/continuation.hpp>
#include <boost/fiber/all.hpp>
#include <iostream>

static void BenchCallCcUnwind(benchmark::State &state)
{
    for (auto _ : state)
    {
        namespace ctx = boost::context;
        int a;
        ctx::continuation source = ctx::callcc(
            [&a](ctx::continuation &&sink)
            {
                a    = 10;
                sink = sink.resume();
                return std::move(sink);
            });
        benchmark::DoNotOptimize(source);
    }
}

BENCHMARK(BenchCallCcUnwind);

static void BenchCallCc(benchmark::State &state)
{
    for (auto _ : state)
    {
        namespace ctx = boost::context;
        int a;
        ctx::continuation source = ctx::callcc(
            [&a](ctx::continuation &&sink)
            {
                a    = 10;
                sink = sink.resume();
                return std::move(sink);
            });
        source.resume();
        benchmark::DoNotOptimize(source);
    }
}

BENCHMARK(BenchCallCc);

static void BenchFiberCreateDetach(benchmark::State &state)
{
    namespace fibers = boost::fibers;
    for (auto _ : state)
    {
        fibers::fiber(fibers::launch::dispatch, []() {}).detach();
    }
}

BENCHMARK(BenchFiberCreateDetach);

static void BenchFiberCreateJoin(benchmark::State &state)
{
    namespace fibers = boost::fibers;
    for (auto _ : state)
    {
        fibers::fiber(fibers::launch::dispatch, []() {}).join();
    }
}

BENCHMARK(BenchFiberCreateJoin);

static void BenchFiberUnBufferChannel(benchmark::State &state)
{
    namespace fibers = boost::fibers;
    fibers::unbuffered_channel<int> ch;
    fibers::fiber reader{fibers::launch::dispatch, [&ch]()
                         {
                             for (auto &it : ch)
                             {
                             }
                         }};
    for (auto _ : state)
    {
        ch.push(1);
    }
    ch.close();
    reader.join();
}

BENCHMARK(BenchFiberUnBufferChannel);

static void BenchFiberBufferChannel(benchmark::State &state)
{
    namespace fibers = boost::fibers;
    fibers::buffered_channel<int> ch(1024);
    fibers::fiber reader{fibers::launch::dispatch, [&ch]
                         {
                             for (auto &it : ch)
                             {
                             }
                         }};
    for (auto _ : state)
    {
        ch.push(1);
    }
    ch.close();
    reader.join();
}

BENCHMARK(BenchFiberBufferChannel);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
