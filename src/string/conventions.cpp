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

#include <absl/strings/numbers.h>
#include <benchmark/benchmark.h>
#include <bits/charconv.h>
#include <boost/lexical_cast.hpp>
#include <charconv>
#include <cmath>
#include <random>
#include <thread>

uint64_t tag = 1234567890123456;
float tagf   = M_PI;

std::string stag  = "1234567890123456";
std::string stagf = std::to_string(M_PI);

template <typename T> static void BenchToString(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::to_string(tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchToString, uint64_t);

template <typename T> static void BenchToStringF(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::to_string(tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchToStringF, float);

template <typename T> static void BenchBoostStringFrom(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(tag);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostStringFrom, std::string);
template <typename T> static void BenchBoostStringTof(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(tagf);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostStringTof, std::string);

template <typename T> static void BenchStdToChars(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::string buff(std::__detail::__to_chars_len(tag), '\0');
        std::__detail::__to_chars_10_impl(buff.data(), buff.size(), tag);
        benchmark::DoNotOptimize(buff);
    }
}
BENCHMARK_TEMPLATE(BenchStdToChars, uint64_t);

template <typename T> static void BenchStdToCharsF(benchmark::State &state)
{
    for (auto _ : state)
    {
        const int __n = __gnu_cxx::__numeric_traits<T>::__max_exponent10 + 20;
        std::array<char, __n> buff;
        auto r = std::to_chars(buff.data(), buff.data() + buff.size(), tagf);
        std::string ret(buff.data(), r.ptr);
        benchmark::DoNotOptimize(r);
        benchmark::DoNotOptimize(ret);
    }
}

BENCHMARK_TEMPLATE(BenchStdToCharsF, float);

template <typename T> static void BenchString2Int(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::stoull(stag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchString2Int, uint64_t);

template <typename T> static void BenchString2Float(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::stof(stagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchString2Float, float);

template <typename T> static void BenchBoostFromString(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(stag);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostFromString, uint64_t);
template <typename T> static void BenchBoostFromStringF(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(stagf);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostFromStringF, float);

template <typename T> static void BenchStdFromChars(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r{};
        std::from_chars(stag.data(), stag.data() + stag.size(), r);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchStdFromChars, uint64_t);

template <typename T> static void BenchStdFromCharsF(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r{};
        std::from_chars(stagf.data(), stagf.data() + stagf.size(), r);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStdFromCharsF, float);

int main(int argc, char **argv)
{
    tag   = std::mt19937_64{std::random_device{}()}();
    tagf  = std::mt19937_64{std::random_device{}()}();
    stag  = std::to_string(std::mt19937_64{std::random_device{}()}());
    stagf = std::to_string(std::mt19937_64{std::random_device{}()}());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    std::__throw_bad_array_new_length();
    return 0;
}