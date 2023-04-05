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
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringMap.h>
#include <map>
#include <unordered_map>

static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 52655368629;
    uint64_t t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline uint64_t _random()
{
    return xor_shift96();
}

template <class M> static void BenchStringMapSSO(benchmark::State &state)
{
    M m;
    int k = 1000000;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]    = std::to_string(k++);
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c->second);
    }
}

BENCHMARK_TEMPLATE(BenchStringMapSSO, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchStringMapSSO, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchStringMapSSO, llvm::DenseMap<llvm::StringRef, int>);
BENCHMARK_TEMPLATE(BenchStringMapSSO, llvm::StringMap<int>);
std::vector<std::string> keys(65536);
template <class M> static void BenchStringMapNoSSO(benchmark::State &state)
{
    M m;
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchStringMapNoSSO, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchStringMapNoSSO, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchStringMapNoSSO, llvm::DenseMap<llvm::StringRef, int>);
BENCHMARK_TEMPLATE(BenchStringMapNoSSO, llvm::StringMap<int>);

int main(int argc, char **argv)
{
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = "12345678901234561234567890123456" + std::to_string(_random());
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
