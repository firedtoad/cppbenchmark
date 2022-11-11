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

#include <benchmark/benchmark.h>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>

const auto &AddTag(const std::string_view &strTag)
{
    static std::vector<std::string> vecTags;
    vecTags.clear();
    vecTags.push_back(strTag.data());
    return strTag;
}

std::string &AddTag(std::string &&strTag)
{
    static std::vector<std::string> vecTags;
    vecTags.clear();
    vecTags.push_back(std::move(strTag));
    return strTag;
}

static void BenchAddConst(benchmark::State &state)
{
    std::string tag = "123456789012345";
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(AddTag(tag));
    }
}

BENCHMARK(BenchAddConst);

static void BenchAddRaw(benchmark::State &state)
{
    std::string_view strTag = "123456789012345";
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(AddTag(strTag));
    }
}

BENCHMARK(BenchAddRaw);

static void BenchAddRValue(benchmark::State &state)
{
    auto f = []() { return std::string{"123456789012345"}; };
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(AddTag(f()));
    }
}

BENCHMARK(BenchAddRValue);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}