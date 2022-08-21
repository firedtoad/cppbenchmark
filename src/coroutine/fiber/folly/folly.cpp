#include <benchmark/benchmark.h>
#include <folly/fibers/FiberManagerMap.h>
#include <folly/io/async/EventBase.h>

#include <iostream>

static void BenchFiberCreatePost(benchmark::State &state)
{
    folly::EventBase evb;
    auto& fiberManager = folly::fibers::getFiberManager(evb);
    for (auto _ : state)
    {
        fiberManager.addTask([&] {});
    }
    evb.loop();
}

BENCHMARK(BenchFiberCreatePost);
static void BenchFiberCreateEager(benchmark::State &state)
{
    folly::EventBase evb;
    auto& fiberManager = folly::fibers::getFiberManager(evb);
    for (auto _ : state)
    {
        fiberManager.addTaskEager([&] {});
    }
    evb.loop();
}

BENCHMARK(BenchFiberCreateEager);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
