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
#include <stdint.h>

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

static inline unsigned long random_()
{
    return xorshf96();
}

uint32_t count(uint32_t n)
{
    unsigned int count = 0;
    while (n > 0)
    { // until all bits are zero
        count++;
        n &= n - 1; // shift bits, removing lower bit
    }
    return count;
}

uint32_t bcount(uint32_t n)
{
    return __builtin_popcount(n);
}

static void BM_count(benchmark::State &state)
{

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(count(random_()));
    }
}

BENCHMARK(BM_count);
static void BM_bcount(benchmark::State &state)
{

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(bcount(random_()));
    }
}

BENCHMARK(BM_bcount);

struct S
{
    S()
    {
        std::atexit([] { std::cout << __PRETTY_FUNCTION__ << '\n'; });
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
    ~S()
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
};

S &CreateS()
{
    auto p = new S();
    return *p;
}

int main(int argc, char **argv)
{

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}