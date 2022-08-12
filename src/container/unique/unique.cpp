//
// Created by zhangwenhao.101 on 2022/8/12.
//
#include <benchmark/benchmark.h>
#include <vector>
#include <set>
#include <unordered_set>

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long        t;

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

static void BM_SortUnique(benchmark::State &state)
{
    std::vector<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec=vi;
        std::sort( vec.begin(), vec.end() );
        vec.erase( std::unique( vec.begin(), vec.end() ), vec.end() );
    }
}

template<class S>
static void BM_UniqueConstructor(benchmark::State &state)
{
    std::vector<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec=vi;
        S s( vec.begin(), vec.end() );
        vec.assign( s.begin(), s.end() );
        std::sort( vec.begin(), vec.end() );
    }
}

template<class S>
static void BM_UniqueManually(benchmark::State &state)
{
    std::vector<int> vi(state.range(0));
    for (auto &it : vi)
    {
        it = random_() % state.range();
    }
    for (auto _ : state)
    {
        auto vec=vi;
        S s;
        for (int i : vec)
            s.insert(i);
        vec.assign( s.begin(), s.end() );
        std::sort( vec.begin(), vec.end() );
    }
}

BENCHMARK(BM_SortUnique)->Range(128,1<<20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor,std::set<int>)->Range(128,1<<20);
BENCHMARK_TEMPLATE(BM_UniqueConstructor,std::unordered_set<int>)->Range(128,1<<20);
BENCHMARK_TEMPLATE(BM_UniqueManually,std::set<int>)->Range(128,1<<20);
BENCHMARK_TEMPLATE(BM_UniqueManually,std::unordered_set<int>)->Range(128,1<<20);


int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}