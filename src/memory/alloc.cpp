
#include <benchmark/benchmark.h>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <vector>

static void BM_AllocSize(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto p = malloc(state.range(0));
        free(p);
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_AllocSize)->Range(1,1<<20);

int CallFunction(const std::function<void()> &fun)
{
    fun();
    return 0;
}

template <class F> int CallLambda(F &&fun)
{
    fun();
    return 0;
}

static void BM_FunctionStack(benchmark::State &state)
{
    int *p = new int;
    std::unordered_map<int, int> umap;
    for (auto _ : state)
    {
        auto ret = CallFunction(
            [p, &umap]
            {
                benchmark::DoNotOptimize(p);
                benchmark::DoNotOptimize(umap);
            });
        benchmark::DoNotOptimize(ret);
    }
    delete p;
}
BENCHMARK(BM_FunctionStack);

static void BM_FunctionHeap(benchmark::State &state)
{
    int *p  = new int;
    int *p1 = new int;
    std::unordered_map<int, int> umap;
    for (auto _ : state)
    {
        auto ret = CallFunction(
            [p,p1,&umap]
            {
                benchmark::DoNotOptimize(p);
                benchmark::DoNotOptimize(p1);
                benchmark::DoNotOptimize(umap);
            });
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_FunctionHeap);

static void BM_Lambda(benchmark::State &state)
{
    int *p  = new int;
    int *p1 = new int;
    std::unordered_map<int, int> umap;

    for (auto _ : state)
    {
        auto ret = CallLambda(
            [p,p1,&umap]
            {
                benchmark::DoNotOptimize(umap);
                benchmark::DoNotOptimize(p);
                benchmark::DoNotOptimize(p1);
            });
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK(BM_Lambda);

int main(int argc, char **argv)
{
    std::cout<<std::_Function_base::_M_max_size<<'\n';
    std::cout<<std::_Function_base::_M_max_align<<'\n';
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    std::map<int,int > m1;
    std::map<int,int > m2;
    if(m1==m2)
    {

    }
    return 0;
}