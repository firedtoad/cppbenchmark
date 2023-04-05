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

#include <benchmark/benchmark.h>
#include <vector>
#include <atomic>

static void BM_Reorder(benchmark::State &state)
{
    int64_t range = state.range(0);
    for (auto _ : state)
    {
        float sum = 0;
        for (int32_t n = 0; n < range; n++)
        {
            sum += (n * 0.01);
            sum += (n / 100.0);
        }
        benchmark::DoNotOptimize(sum);
    }
}

template <int order> struct Order
{
    void operator()()
    {
        std::atomic_thread_fence((std::memory_order)order);
    }
};

template <typename F> static void BM_NoReorder(benchmark::State &state)
{
    int64_t range = state.range(0);
    F fn;
    for (auto _ : state)
    {
        float sum = 0;
        for (int32_t n = 0; n < range; n++)
        {
            sum += (n * 0.01);
            fn();
            sum += (n / 100.0);
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BM_Reorder)->Range(1024 * 1024, 1024 * 1024);
BENCHMARK_TEMPLATE(BM_NoReorder, Order<std::memory_order_relaxed>)->Range(1024 * 1024, 1024 * 1024);
BENCHMARK_TEMPLATE(BM_NoReorder, Order<std::memory_order_consume>)->Range(1024 * 1024, 1024 * 1024);
BENCHMARK_TEMPLATE(BM_NoReorder, Order<std::memory_order_acquire>)->Range(1024 * 1024, 1024 * 1024);
BENCHMARK_TEMPLATE(BM_NoReorder, Order<std::memory_order_release>)->Range(1024 * 1024, 1024 * 1024);
BENCHMARK_TEMPLATE(BM_NoReorder, Order<std::memory_order_acq_rel>)->Range(1024 * 1024, 1024 * 1024);
BENCHMARK_TEMPLATE(BM_NoReorder, Order<std::memory_order_seq_cst>)->Range(1024 * 1024, 1024 * 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}