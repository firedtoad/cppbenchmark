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
#include <map>
#include <unordered_map>

using Handler = int (*)();
static std::map<int, Handler> map_handler;
static std::unordered_map<int, Handler> unmap_handler;

// static std::array<Handler, 65536> array_handler;
static Handler array_handler[65536];

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long _random()
{
    return xorshf96();
}

static void BM_map(benchmark::State &state)
{
    for (auto i = 0; i < state.range(); i++)
    {
        map_handler[i] = []() -> int { return 1; };
    }

    for (auto _ : state)
    {
        auto idx      = _random() % state.range();
        const auto it = map_handler.find(idx);
        if (it != map_handler.end())
        {
            auto r=it->second();
            benchmark::DoNotOptimize(r);
        }
    }
}

static void BM_unmap(benchmark::State &state)
{
    unmap_handler.reserve(state.range());
    for (auto i = 0; i < state.range(); i++)
    {
        unmap_handler[i] = []() -> int { return 1; };
    }

    for (auto _ : state)
    {
        auto idx      = _random() % state.range();
        const auto it = unmap_handler.find(idx);
        if (it != unmap_handler.end())
        {
            auto r=it->second();
            benchmark::DoNotOptimize(r);
        }
    }
}

static void BM_array(benchmark::State &state)
{
    for (auto i = 0; i < state.range(); i++)
    {
        array_handler[i] = []() -> int { return 1; };
    }
    for (auto _ : state)
    {
        auto idx = _random() % state.range();
        if (array_handler[idx])
        {
            auto r=array_handler[idx]();
            benchmark::DoNotOptimize(r);
        }
    }
}

BENCHMARK(BM_map)->Range(1, 1 << 16);
BENCHMARK(BM_unmap)->Range(1, 1 << 16);
BENCHMARK(BM_array)->Range(1, 1 << 16);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}