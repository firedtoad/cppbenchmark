#include <benchmark/benchmark.h>
#include <iostream>
#include <map>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/StringMap.h>

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

template <class M> static void BenchStringMapSSO(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    int k = 1000000;
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

BENCHMARK_TEMPLATE(BenchStringMapSSO, llvm::StringMap<int>);

template <class M> static void BenchStringMapNoSSO(benchmark::State &state)
{
    M m;
    std::vector<std::string> keys(65536);
    int k = 1000000;
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]    = "12345678901234561234567890123456" + std::to_string(k++);
        m[keys[i].c_str()] = i;
    }

    for (auto _ : state)
    {
        auto kIndex = _random() % 65536;
        auto c      = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchStringMapNoSSO,llvm::StringMap<int>);
int main(int argc, char **argv)
{

    llvm::StringMap<int> m;
    std::vector<std::string> keys(65536);
    int k = 1000000;
    for (auto i = 0; i < 65536; i++)
    {
        keys[i]    = "12345678901234561234567890123456" + std::to_string(k++);
        m[keys[i]] = i;
    }

    auto kIndex = _random() % 65536;
    auto c      = m.find(keys[kIndex].c_str());
    std::cout<<c->first().str()<<c->second<<'\n';
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
