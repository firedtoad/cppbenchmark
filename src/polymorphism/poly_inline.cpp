//
// Created by dietoad@gmail.com on 2022/1/21.
//
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
    delete p;
}

static void BenchVirtualCall(benchmark::State &state)
{
    auto count = state.range(0);
    Base *p    = new Derived{};
    auto sum   = 0;
    for (auto _ : state)
    {
        for (auto i = 0; i < count; i++)
        {
            sum += p->v_func();
            benchmark::DoNotOptimize(sum);
        }
    }
    delete p;
}

BENCHMARK(BenchPureCall)->Range(1024, 1024);
BENCHMARK(BenchVirtualCall)->Range(1024, 1024);

int main(int argc, char **argv)
{

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}