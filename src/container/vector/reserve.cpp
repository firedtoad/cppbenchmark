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

static void BM_insert(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<int> v;
        for (auto i = 0; i < 1024; i++)
        {
            v.push_back(i);
        }
    }
}

static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<int> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.push_back(i);
        }
    }
}

BENCHMARK(BM_insert);
BENCHMARK(BM_reserve);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}