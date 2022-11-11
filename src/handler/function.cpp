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
#include <functional>
#include <vector>

using Handler = int (*)();

static inline uint64_t xor_shift96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 521288629;
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

static void BM_array(benchmark::State &state)
{
    std::array<Handler, 65536> array_handler;
    for (auto i = 0; i < 65536; i++)
    {
        array_handler[i] = []() -> int { return 1; };
    }
    for (auto _ : state)
    {
        auto idx = _random() % 65536;
        if (array_handler[idx])
        {
            benchmark::DoNotOptimize(array_handler[idx]());
        }
    }
}

static void BM_function(benchmark::State &state)
{
    std::vector<std::function<int()>> handlers(65536);
    for (auto i = 0; i < 65536; i++)
    {
        handlers[i] = []() -> int { return 1; };
    }
    for (auto _ : state)
    {
        auto idx = _random() % 65536;
        if (handlers[idx])
        {
            benchmark::DoNotOptimize(handlers[idx]());
        }
    }
}

BENCHMARK(BM_array);
BENCHMARK(BM_function);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}