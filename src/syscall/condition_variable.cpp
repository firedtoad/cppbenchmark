//
// Created by Administrator on 2023/10/15.
//
#include <benchmark/benchmark.h>
#include <condition_variable>
#include <mutex>
#include <thread>
static std::condition_variable cv;
static std::mutex mu;
std::atomic_bool bRun = true;
void thread_wait()
{
    std::unique_lock lock(mu);
    do
    {
        cv.wait(lock);
    } while (bRun.load(std::memory_order_acquire));
}

template <size_t wait> static void BenchCondition(benchmark::State &state)
{
    int n = 0;
    for (auto _ : state)
    {
        if constexpr (wait == 0)
        {
            cv.notify_one();
        }
        else if (wait == 1)
        {
            {
                std::unique_lock lock(mu);
                benchmark::DoNotOptimize(lock);
            }
            cv.notify_one();
        }
        else
        {
            std::unique_lock lock(mu);
            benchmark::DoNotOptimize(lock);
            cv.notify_one();
        }
    }
    benchmark::DoNotOptimize(n);
}
BENCHMARK_TEMPLATE(BenchCondition, 0);
BENCHMARK_TEMPLATE(BenchCondition, 1);
BENCHMARK_TEMPLATE(BenchCondition, 2);

int main(int argc, char **argv)
{
    std::thread waiter(thread_wait);
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    bRun.store(false, std::memory_order_release);
    cv.notify_one();
    waiter.join();
    return 0;
}