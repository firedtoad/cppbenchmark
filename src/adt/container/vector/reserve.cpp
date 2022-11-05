#include <benchmark/benchmark.h>
#include <llvm/ADT/SmallVector.h>
#include <numeric>
#include <vector>

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

template <typename V> static void BM_insert(benchmark::State &state)
{

    for (auto _ : state)
    {
        V v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.emplace_back(i);
        }
        benchmark::DoNotOptimize(v.back());
    }
}
template <typename V> static void BM_reserve(benchmark::State &state)
{
    for (auto _ : state)
    {
        V v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.emplace_back(i);
        }
        benchmark::DoNotOptimize(v.back());
    }
}

//BENCHMARK_TEMPLATE(BM_insert, llvm::SmallVector<uint64_t, 1024>)->Range(1024, 1024);
//BENCHMARK_TEMPLATE(BM_insert, std::vector<uint64_t>)->Range(1024, 1024);
//BENCHMARK_TEMPLATE(BM_reserve, llvm::SmallVector<uint64_t, 1024>)->Range(1024, 1024);
//BENCHMARK_TEMPLATE(BM_reserve, std::vector<uint64_t>)->Range(1024, 1024);

template <typename V> static void BM_Range(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.emplace_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::accumulate(v.begin(), v.end(), 0));
    }
}

//BENCHMARK_TEMPLATE(BM_Range, llvm::SmallVector<uint64_t, 1024>)->Range(1024, 1024);
//BENCHMARK_TEMPLATE(BM_Range, std::vector<uint64_t>)->Range(1024, 1024);

template <typename V> static void BM_erase(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.emplace_back(i);
    }
    for (auto _ : state)
    {
        auto idx   = random_() % v.size();
        auto start = std::next(v.begin(), idx);
        auto val   = *start;
        v.erase(start);
        v.push_back(val);
    }
}

BENCHMARK_TEMPLATE(BM_erase, llvm::SmallVector<uint64_t, 1024>)->Range(1024, 1024);
BENCHMARK_TEMPLATE(BM_erase, std::vector<uint64_t>)->Range(1024, 1024);

template <typename V> static void BM_eraseSwap(benchmark::State &state)
{
    V v;
    v.reserve(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.emplace_back(i);
    }
    for (auto _ : state)
    {
        auto idx   = random_() % v.size();
        auto start = std::next(v.begin(), idx);
        auto val   = *start;
        std::swap(*start, v.back());
        v.pop_back();
        v.push_back(val);
    }
}
BENCHMARK_TEMPLATE(BM_eraseSwap, llvm::SmallVector<uint64_t, 1024>)->Range(1024, 1024);
BENCHMARK_TEMPLATE(BM_eraseSwap, std::vector<uint64_t>)->Range(1024, 1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}