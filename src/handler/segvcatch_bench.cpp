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

#include "segvcatch/segvcatch.h"
#include <benchmark/benchmark.h>

int *p = nullptr;
int *d = new int{0};
static void BM_SegvCatch(benchmark::State &state)
{
    for (auto _ : state)
    {
        try
        {
            *p = 10;
            benchmark::DoNotOptimize(p);
        }
        catch (std::exception &e)
        {

        }
    }
}

BENCHMARK(BM_SegvCatch);

int main(int argc, char **argv)
{
    segvcatch::init_segv();
    segvcatch::init_fpe();
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}