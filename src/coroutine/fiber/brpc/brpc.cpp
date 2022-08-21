#include <benchmark/benchmark.h>
#include <bthread/bthread.h>
#include <iostream>

static void BenchBackground(benchmark::State &state)
{
    for (auto _ : state)
    {
        bthread_t tid{};
        bthread_start_background(
            &tid, nullptr, [](void *) { return (void *)0; }, nullptr);
    }
}

BENCHMARK(BenchBackground);
static void BenchUrgent(benchmark::State &state) {
    for (auto _ : state)
    {
        bthread_t tid{};
        bthread_start_urgent(
            &tid, nullptr, [](void *) { return (void *)0; }, nullptr);
    }
}

BENCHMARK(BenchUrgent);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
