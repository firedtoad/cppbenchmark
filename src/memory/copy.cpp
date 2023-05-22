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
#include <cstring>
#include <openssl/rc5.h>
#include <vector>
static void BenchMemCpy(benchmark::State &state)
{
    std::vector<char> cache;
    cache.resize(state.range(0));
    std::vector<char> dst;
    dst.resize(state.range(0));
    for (auto _ : state)
    {
        memcpy(dst.data(), cache.data(), cache.size());
        benchmark::DoNotOptimize(dst);
    }
}

static void BenchStdCopy(benchmark::State &state)
{
    std::vector<char> cache;
    cache.resize(state.range(0));
    std::vector<char> dst;
    dst.resize(state.range(0));
    for (auto _ : state)
    {
        std::copy(cache.begin(), cache.end(), dst.data());
        benchmark::DoNotOptimize(dst);
    }
}

BENCHMARK(BenchMemCpy)->Range(1, 1024);
BENCHMARK(BenchStdCopy)->Range(1, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}