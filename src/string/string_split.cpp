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

#include <absl/strings/str_split.h>
#include <benchmark/benchmark.h>
#include <boost/algorithm/string.hpp>
#include <functional>
#include <google/protobuf/stubs/strutil.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

void StringSplit(const std::string &str, const std::string &delimiters, std::vector<std::string> &elems)
{
    std::string::size_type pos, prev = 0;
    while ((pos = str.find_first_of(delimiters, prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            elems.emplace_back(str, prev, pos - prev);
        }
        prev = pos + 1;
    }
    if (prev < str.size())
        elems.emplace_back(str, prev, str.size() - prev);
}

void StringSplitStream(const std::string &str, const char &delim, std::vector<std::string> &elems)
{
    thread_local auto ss = std::make_unique<std::istringstream>();
    ss->clear();
    ss->str(str);
    std::string token;
    while (std::getline(*ss, token, delim))
    {
        elems.push_back(token);
    }
}

static void BenchStringSplit(benchmark::State &state)
{
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r           = 0;

    for (auto _ : state)
    {
        std::vector<std::string> vec;
        StringSplit(tag, ",", vec);
        for (auto &i : vec)
        {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchStringSplit)->ThreadRange(1, 8);

static void BenchStringSplitStream(benchmark::State &state)
{
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r           = 0;

    for (auto _ : state)
    {
        std::vector<std::string> vec;
        StringSplitStream(tag, ',', vec);
        for (auto &i : vec)
        {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchStringSplitStream)->ThreadRange(1, 8);

static void BenchBoostSplit(benchmark::State &state)
{
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r           = 0;
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        boost::split(vec, tag, [](char c) { return c = ','; });
        for (auto &i : vec)
        {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchBoostSplit)->ThreadRange(1, 8);

static void BenchABSplit(benchmark::State &state)
{
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r           = 0;
    for (auto _ : state)
    {
        auto vec = absl::StrSplit(tag, ",");
        for (auto &i : vec)
        {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchABSplit)->ThreadRange(1, 8);

static void BenchPBSplit(benchmark::State &state)
{
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r           = 0;
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        google::protobuf::SplitStringAllowEmpty(tag, ",", &vec);
        for (auto &i : vec)
        {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPBSplit)->ThreadRange(1, 8);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}