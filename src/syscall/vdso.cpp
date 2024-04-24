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
#include <chrono>
#include <sys/syscall.h>
#include <unistd.h>

static void BenchVdsoTime(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            auto n = time(nullptr);
            benchmark::DoNotOptimize(n);
        }
    }
}

BENCHMARK(BenchVdsoTime)->Range(1, 1);

static void BenchSyscallTime(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (auto i = 0; i < state.range(0); i++)
        {
            time_t tm{};
            auto n = syscall(SYS_time,&tm);
            benchmark::DoNotOptimize(n);
        }
    }
}

BENCHMARK(BenchSyscallTime)->Range(1, 1);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
