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
#include <unordered_map>

#include <llvm/ADT/IntervalMap.h>
static void BM_Interval(benchmark::State &state)
{
    for (auto _ : state)
    {
        llvm::IntervalMap<int, int>::Allocator alloc;
        llvm::IntervalMap<int, int> m(alloc);
        for (auto i = 0; i < state.range(0); i += 16)
        {
            m.insert(i, i + 16, 10);
        }
    }
}

BENCHMARK(BM_Interval)->Range(1, 1 << 20);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}