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

#include <absl/strings/charconv.h>
#include <absl/strings/numbers.h>
#include <benchmark/benchmark.h>
#include <boost/lexical_cast.hpp>
#include <charconv>
#include <cmath>
#include <google/protobuf/stubs/strutil.h>
#include <random>
#include <sstream>

std::string tag  = "1234567890123456";
std::string tagf = std::to_string(M_PI);
template <typename S, typename T> void StringToNumber(const std::string &str, T &t)
{
    S oss(str);
    oss >> t;
}

template <typename S, typename T> static void BenchStreamStringTo(benchmark::State &state)
{

    T r = 0;
    for (auto _ : state)
    {
        StringToNumber<S, T>(tag, r);
        benchmark::DoNotOptimize(r);
    }
}

template <typename S, typename T> static void BenchStreamStringToF(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        StringToNumber<S, T>(tagf, r);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStreamStringTo, std::stringstream, uint64_t);
BENCHMARK_TEMPLATE(BenchStreamStringTo, std::istringstream, uint64_t);

BENCHMARK_TEMPLATE(BenchStreamStringToF, std::stringstream, float);
BENCHMARK_TEMPLATE(BenchStreamStringToF, std::istringstream, float);

template <typename T> static void BenchStdAtol(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = std::atoll(tag.c_str());
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchStdAtol, uint64_t);

template <typename T> static void BenchStdAtof(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = std::atof(tagf.c_str());
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchStdAtof, float);

template <typename T> static void BenchStdStol(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = std::stoull(tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchStdStol, uint64_t);

template <typename T> static void BenchStdStof(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = std::stof(tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchStdStof, float);

template <typename T> static void BenchStdCharConvInt(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r{};
        std::from_chars(tag.data(), tag.data() + tag.size(), r, 10);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStdCharConvInt, uint64_t);

template <typename T> static void BenchABCharConvIntF(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r{};
        absl::from_chars(tagf.data(), tagf.data() + tagf.size(), r);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchABCharConvIntF, float);

template <typename T> static void BenchBoostStringTo(benchmark::State &state)
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

BENCHMARK_TEMPLATE(BenchBoostStringTo, uint64_t);

BENCHMARK_TEMPLATE(BenchBoostStringTof, float);

template <typename T> static void BenchAbSimpleToInt(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        if (absl::SimpleAtoi(tag, &r))
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
        if (absl::SimpleAtof(tagf, &r))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbSimpleToFloat, float);

template <typename T> static void BenchPBStrAppend(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::string s;
        google::protobuf::StrAppend(&s, tag);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK_TEMPLATE(BenchPBStrAppend, uint64_t);

template <typename T> static void BenchPBStrAppendF(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::string s;
        google::protobuf::StrAppend(&s, tagf);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK_TEMPLATE(BenchPBStrAppendF, float);

int main(int argc, char **argv)
{

    tag  = std::to_string(std::mt19937_64{std::random_device{}()}());
    tagf = std::to_string(std::mt19937_64{std::random_device{}()}());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}