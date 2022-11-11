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
    virtual ~Base() {}
    virtual uint64_t v_func() = 0;
};

class Derived : public Base
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

template <class T> class CRTP
{
  public:
    uint64_t v_func()
    {
        return static_cast<T *>(this)->v_func();
    }
};

class CDerived : public CRTP<CDerived>
{
    uint64_t y;

  public:
    uint64_t v_func()
    {
        return y++;
    }
};

static void BenchPureCall(benchmark::State &state)
{
    auto count  = state.range(0);
    Derived **p = new Derived *[count] {};
    for (auto i = 0; i < count; i++)
    {
        p[i] = new Derived{};
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < count; i++)
        {
            sum += p[i]->pure();
        }
    }
    for (auto i = 0; i < count; i++)
    {
        delete p[i];
    }
    delete[] p;
    benchmark::DoNotOptimize(sum);
}

static void BenchVirtualCall(benchmark::State &state)
{
    auto count = state.range(0);
    Base **p   = new Base *[count] {};
    for (auto i = 0; i < count; i++)
    {
        p[i] = new Derived{};
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < count; i++)
        {
            sum += p[i]->v_func();
        }
    }
    for (auto i = 0; i < count; i++)
    {
        delete p[i];
    }
    delete[] p;
    benchmark::DoNotOptimize(sum);
}

static void BenchCRTP(benchmark::State &state)
{

    auto count         = state.range(0);
    CRTP<CDerived> **p = new CRTP<CDerived> *[count] {};
    for (auto i = 0; i < count; i++)
    {
        p[i] = new CDerived{};
    }
    auto sum = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < count; i++)
        {
            sum += p[i]->v_func();
        }
    }
    for (auto i = 0; i < count; i++)
    {
        delete p[i];
    }
    delete[] p;
    benchmark::DoNotOptimize(sum);
}

BENCHMARK(BenchPureCall)->Range(1024, 1024);
BENCHMARK(BenchVirtualCall)->Range(1024, 1024);
BENCHMARK(BenchCRTP)->Range(1024, 1024);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}