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
#include <iostream>
#include <map>
#include <memory>
#include <vector>

struct S
{
    uint64_t s;
    //    __attribute__((always_inline)) void operator delete[] (void * p,size_t sz){
    //        operator delete(p);
    //    }
};

static void BM_Delete(benchmark::State &state)
{
    int row = state.range(0);

    for (auto _ : state)
    {
        for (auto i = 0; i < row; i++)
        {
            auto p = new S[row];
            benchmark::DoNotOptimize(p);
            delete[] p;
        }
    }
}

static void BM_DeleteSized(benchmark::State &state)
{
    int row = state.range(0);
    for (auto _ : state)
    {
        for (auto i = 0; i < row; i++)
        {
            std::vector<S> p;
            p.reserve(row);
            benchmark::DoNotOptimize(p);
        }
    }
}

BENCHMARK(BM_Delete)->Range(1, 1 << 20);
BENCHMARK(BM_DeleteSized)->Range(1, 1 << 20);

int main(int argc, char **argv)
{
    auto p = new S[1];
    benchmark::DoNotOptimize(p);
    delete[] p;
    std::map<int, int> mp;
    std::set<int> st;
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}