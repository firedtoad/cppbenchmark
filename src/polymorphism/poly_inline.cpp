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

class Base
{
  public:
    virtual uint64_t v_func() = 0;
};

class Derived final : public Base
{
  protected:
    uint64_t y;

  public:
    uint64_t pure()
    {
        return y++;
    }
    uint64_t v_func()
    {
        return y++;
    }
};

static void BenchPureCall(benchmark::State &state)
{
    auto count = state.range(0);
    Derived *p = new Derived{};
    auto sum   = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < count; i++)
        {
            sum += p->pure();
            benchmark::DoNotOptimize(sum);
        }
    }
}

static void BenchVirtualCall(benchmark::State &state)
{
    auto count = state.range(0);
    Derived *d    = new Derived{};
    Base *p=d;
    auto sum   = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < count; i++)
        {
            sum += p->v_func();
            benchmark::DoNotOptimize(sum);
        }
    }
    delete d;
}

BENCHMARK(BenchPureCall)->Range(1024, 1024);
BENCHMARK(BenchVirtualCall)->Range(1024, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}