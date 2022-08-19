#include <benchmark/benchmark.h>
#include <boost/context/continuation.hpp>
#include <boost/context/fiber.hpp>
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

BENCHMARK(BenchCallCcUnwind)->Range(1024, 1024);

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

BENCHMARK(BenchCallCc)->Range(1024, 1024);

static void BenchFiberCreate(benchmark::State &state)
{
    for (auto _ : state)
    {
        namespace fibers = boost::fibers;
        fibers::fiber fb{fibers::launch::post,
                         [](int x)
                         {
                         },
                         10};
        fb.join();
    }
}

BENCHMARK(BenchFiberCreate)->Range(1024, 1024);

int main(int argc, char **argv)
{
    {
        namespace fibers = boost::fibers;
        fibers::fiber fb{fibers::launch::post,
                         [](int x)
                         {
                             std::cout << x;
                             x = 100;
                         },
                         10};
        fb.join();
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
