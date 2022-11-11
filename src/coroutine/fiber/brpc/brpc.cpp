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
#include <bthread/bthread.h>
#include <iostream>

static void BenchBackground(benchmark::State &state)
{
    for (auto _ : state)
    {
        bthread_t tid{};
        bthread_start_background(
            &tid, &BTHREAD_ATTR_SMALL, [](void *) { return (void *)0; }, nullptr);
    }
}

BENCHMARK(BenchBackground);
static void BenchUrgent(benchmark::State &state)
{
    for (auto _ : state)
    {
        bthread_t tid{};
        bthread_start_urgent(
            &tid, &BTHREAD_ATTR_SMALL, [](void *) { return (void *)0; }, nullptr);
    }
}

BENCHMARK(BenchUrgent);
DECLARE_int32(guard_page_size);
int main(int argc, char **argv)
{
    //    FLAGS_guard_page_size = 0;
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
