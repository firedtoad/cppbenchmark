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
                             for(auto &it:ch)
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
                            for(auto &it:ch)
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
