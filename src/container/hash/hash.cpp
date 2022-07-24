//
// Created by Administrator on 2022/06/28.
//

#include "butil/containers/flat_map.h"
#include "flat_hash_map.hpp"
#include "internal/city.h"
#include "internal/murmurhash3.h"
#include "internal/wyhash.h"
#include "internal/xxhash.h"
#include "tsl/robin_map.h"
#include <benchmark/benchmark.h>
#include <string>
#include <vector>
constexpr int N = 100;

void init(std::vector<std::string> &v1)
{
    v1.resize(N);
    for (auto i = 0; i < N; i++)
    {
        v1[i] = std::to_string(i);
    }
}

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

static void BM_StdHash(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::hash<std::string>{}(vec[idx]));
    }
}

BENCHMARK(BM_StdHash);

static void BM_StdHashFnv(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::_Fnv_hash_impl{}.hash(vec[idx]));
    }
}

BENCHMARK(BM_StdHashFnv);

static void BM_CityHash(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        auto r = CityHash64(vec[idx].c_str(), vec[idx].size());
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_CityHash);

static void BM_Murmur(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        auto out = 0;
        butil::MurmurHash3_x86_32(vec[idx].c_str(), vec[idx].size(), 0, &out);
        benchmark::DoNotOptimize(out);
    }
}

BENCHMARK(BM_Murmur);

static void BM_WyHash(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        auto r = wyhash(vec[idx].c_str(), vec[idx].size(), 0, _wyp);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_WyHash);

static void BM_XXHash(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        auto r = XXH64(vec[idx].c_str(), vec[idx].size(), 0);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_XXHash);

static void BM_DefaultHash(benchmark::State &state)
{
    std::vector<std::string> vec;
    init(vec);
    auto idx = random_() % vec.size();
    for (auto _ : state)
    {
        auto r = butil::DefaultHasher<std::string>{}(vec[idx]);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_DefaultHash);

struct Hasher
{
    size_t operator()(const std::string &str) const
    {
        //    return butil::DefaultHasher<std::string>{}(str);
        return wyhash(str.c_str(), str.size(), 0, _wyp);
        //    return XXH64(str.c_str(), str.size(), 0);
        return std::hash<std::string>{}(str);
        //    size_t out = 0;
        //    butil::MurmurHash3_x86_32(str.c_str(), str.size(), 0, &out);
        //    return out;
    }
};

template <class M> static void BenchUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = std::to_string(random_());
        //    auto sKey = std::to_string(i);
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = random_() % 65536;
        auto c      = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(c->first);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int, Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int, Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int, Hasher>);

static void BenchFlatMapString(benchmark::State &state)
{
    butil::FlatMap<std::string, int> m;
    m.init(65536);
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = std::to_string(random_());
        //    auto sKey = std::to_string(i);
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = random_() % 65536;
        auto c      = m.seek(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BenchFlatMapString);

static void BenchFlatMapStringStandHash(benchmark::State &state)
{
    butil::FlatMap<std::string, int, std::hash<std::string>> m;
    m.init(65536);
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = std::to_string(random_());
        //    auto sKey = std::to_string(i);
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = random_() % 65536;
        auto c      = m.seek(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BenchFlatMapStringStandHash);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}