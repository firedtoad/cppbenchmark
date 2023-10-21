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

#include "butil/containers/flat_map.h"
#include "flat_hash_map.hpp"
#include "internal/city.h"
#include "internal/murmurhash3.h"
#include "internal/wyhash.h"
#include "internal/xxhash.h"
#include "parallel_hashmap/phmap.h"
#include "tsl/robin_map.h"
#include <absl/container/flat_hash_map.h>
#include <absl/hash/hash.h>
#include <benchmark/benchmark.h>
#include <string>
#include <vector>
constexpr int N = 1024;

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
std::vector<std::string> vec;
void init(std::vector<std::string> &v1)
{
    v1.resize(N);
    for (auto i = 0; i < N; i++)
    {
        v1[i] = std::to_string(random_())+std::to_string(random_()) + std::to_string(random_());
    }
}

static void BM_StdHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto r=std::_Hash_impl::hash(vec[idx]);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_StdHash);

static void BM_StdHashFnv(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto r=std::_Fnv_hash_impl::hash(vec[idx]);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_StdHashFnv);

static void BM_CityHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto r = CityHash64(vec[idx].c_str(), vec[idx].size());
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_CityHash);

static void BM_Murmur(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto out = 0;
        butil::MurmurHash3_x86_32(vec[idx].c_str(), vec[idx].size(), 0, &out);
        benchmark::DoNotOptimize(out);
    }
}

BENCHMARK(BM_Murmur);

static void BM_WyHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto r = wyhash(vec[idx].c_str(), vec[idx].size(), 0, _wyp);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_WyHash);

static void BM_XXHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto r = XXH64(vec[idx].c_str(), vec[idx].size(), 0);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_XXHash);

static void BM_AbseilHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
        auto r = absl::Hash<std::string>{}(vec[idx]);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_AbseilHash);

static void BM_DefaultHash(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto idx = random_() % vec.size();
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
std::vector<std::string> keys(65536);
template <class M> static void BenchUnOrderMapString(benchmark::State &state)
{
    M m;
    m.reserve(65536);

    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = random_() % 65536;
        auto c      = m.find(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, std::unordered_map<std::string, int, Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, ska::flat_hash_map<std::string, int, Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, phmap::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, phmap::flat_hash_map<std::string, int, Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, absl::flat_hash_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, absl::flat_hash_map<std::string, int, absl::Hash<std::string>>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, absl::flat_hash_map<std::string, int, Hasher>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int>);
BENCHMARK_TEMPLATE(BenchUnOrderMapString, tsl::robin_map<std::string, int, Hasher>);
template <class M>
static void BenchFlatMapString(benchmark::State &state)
{
    M m;
    m.init(65536);
    for (auto i = 0; i < 65536; i++)
    {
        m[keys[i]] = i;
    }
    for (auto _ : state)
    {
        auto kIndex = random_() % 65536;
        auto c      = m.seek(keys[kIndex]);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchFlatMapString,butil::FlatMap<std::string, int>);
BENCHMARK_TEMPLATE(BenchFlatMapString,butil::FlatMap<std::string, int, std::hash<std::string>>);
BENCHMARK_TEMPLATE(BenchFlatMapString,butil::FlatMap<std::string,int, Hasher>);


int main(int argc, char **argv)
{
    init(vec);
    for (auto i = 0; i < 65536; i++)
    {
        keys[i] = std::to_string(random_())+std::to_string(random_());
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}