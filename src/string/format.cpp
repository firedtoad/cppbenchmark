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
#include <absl/strings/str_format.h>
#include <benchmark/benchmark.h>
#include <boost/lexical_cast.hpp>
#include <charconv>
#include <cmath>
#include <folly/Conv.h>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
uint64_t tag = 1234567890123456;
float tagf   = M_PI;

std::string stag  = "1234567890123456";
std::string stagf = std::to_string(M_PI);

template <typename S, typename T> void NumberToString(T &t, std::string &str)
{
    S oss(str);
    oss << t;
    str = oss.str();
}

template <typename S, typename T> static void BenchStreamStringFrom(benchmark::State &state)
{
    std::string r;
    for (auto _ : state)
    {
        NumberToString<S, T>(tag, r);
        benchmark::DoNotOptimize(r);
    }
}

template <typename S, typename T> static void BenchStreamStringFromF(benchmark::State &state)
{
    std::string r;
    for (auto _ : state)
    {
        NumberToString<S, T>(tagf, r);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStreamStringFrom, std::stringstream, uint64_t);
BENCHMARK_TEMPLATE(BenchStreamStringFrom, std::ostringstream, uint64_t);

BENCHMARK_TEMPLATE(BenchStreamStringFromF, std::stringstream, float);
BENCHMARK_TEMPLATE(BenchStreamStringFromF, std::ostringstream, float);

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

template <typename T> static void BenchStdToChars(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buf[64]{};
        auto r = std::to_chars(buf, buf + 64, tag, 10);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStdToChars, uint64_t);

template <typename T> static void BenchBoostStringFrom(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(tag);
        benchmark::DoNotOptimize(r);
    }
}
template <typename T> static void BenchBoostStringTof(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(tagf);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostStringFrom, std::string);

BENCHMARK_TEMPLATE(BenchBoostStringTof, std::string);

template <typename T> static void BenchAbFastIntToBuffer(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        char buf[64]{};
        if (absl::numbers_internal::FastIntToBuffer(tag, buf))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbFastIntToBuffer, uint64_t);

template <typename T> static void BenchAbSixDigitsToBuffer(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        char buf[64]{};
        if (absl::numbers_internal::SixDigitsToBuffer(tagf, buf))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbSixDigitsToBuffer, float);

template <typename T> static void BenchAbStrFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = absl::StrFormat("%d", tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbStrFormat, uint64_t);

template <typename T> static void BenchAbStrFormatF(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = absl::StrFormat("%f", tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbStrFormatF, float);

template <typename T> static void BenchFollyTo(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = folly::to<std::string>(tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchFollyTo, uint64_t);

template <typename T> static void BenchFollyTof(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = folly::to<std::string>(tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchFollyTof, float);

template <typename T> static void BenchFollyToInt(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = folly::to<T>(stag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchFollyToInt, uint64_t);

template <typename T> static void BenchFollyToFloat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = folly::to<T>(stagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchFollyToFloat, float);

template <typename T> static void BenchAbSimpleToInt(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        if (absl::SimpleAtoi(stag, &r))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbSimpleToInt, uint64_t);

template <typename T> static void BenchAbSimpleToFloat(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        if (absl::SimpleAtof(stagf, &r))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbSimpleToFloat, float);

static std::map<int, int> f;

auto StringSplitStream(const std::string &str, const char &delim, uint64_t i)
{
    thread_local std::istringstream ss;
    ss.clear();
    ss.str(str);
    std::string token;
    std::vector<std::string> ret;
    while (std::getline(ss, token, delim))
    {
        ret.push_back(token);
    }
    return ret;
}
static void BenchStringSplitStream(benchmark::State &state)
{
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r           = 0;
    int nx          = rand();
    for (auto _ : state)
    {
        auto vec = StringSplitStream(tag, ',', nx);
        for (auto &i : vec)
        {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchStringSplitStream)->ThreadRange(1, 8);

int main(int argc, char **argv)
{
    srand(time(nullptr));
    tag   = std::mt19937_64{std::random_device{}()}();
    tagf  = std::mt19937_64{std::random_device{}()}();
    stag  = std::to_string(std::mt19937_64{std::random_device{}()}());
    stagf = std::to_string(std::mt19937_64{std::random_device{}()}());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    std::cout << f.size() << '\n';
    for (auto &it : f)
    {
        std::cout << it.first << ':' << it.second << '\n';
    }
    return 0;
}