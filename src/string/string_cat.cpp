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

#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <benchmark/benchmark.h>
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <google/protobuf/stubs/strutil.h>
#include <random>
#include <sstream>

std::string tag  = "1234567890123456";
std::string tag1 = "1234567890123456";

static void BenchStrNCat(benchmark::State &state)
{
    for (auto _ : state)
    {
        char dest[128]{};
        strncat(dest, tag.c_str(), tag.size());
        strncat(&dest[tag.size()], ":", 2);
        strncat(&dest[tag.size() + 1], tag1.c_str(), tag1.size());
        benchmark::DoNotOptimize(dest);
    }
}
BENCHMARK(BenchStrNCat);

static void BenchStdStringCat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = tag;
        r += ":";
        r += tag1;
        r += tag1;
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchStdStringCat);

static void BenchStreamStringCat(benchmark::State &state)
{

    for (auto _ : state)
    {
        std::stringstream ss;
        ss << tag << ":" << tag1 << tag1;
        auto r = ss.str();
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchStreamStringCat);

static void BenchBoostStrCat(benchmark::State &state)
{

    for (auto _ : state)
    {
        auto r = boost::join<std::vector<std::string>>({tag, ":", tag1}, "");
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchBoostStrCat);

static void BenchAbStrCat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = absl::StrCat(tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchAbStrCat);

static void BenchPBStrCat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = google::protobuf::StrCat(tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchPBStrCat);

static void BenchAbStrFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = absl::StrFormat("%s%s%s%s", tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchAbStrFormat);

static void BenchFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = fmt::format("{}{}{}{}", tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchFormat);

int main(int argc, char **argv)
{

    tag  = std::to_string(std::mt19937_64{std::random_device{}()}());
    tag1 = std::to_string(std::mt19937_64{std::random_device{}()}());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}