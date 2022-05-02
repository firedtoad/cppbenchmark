//
// Created by Administrator on 2022/05/01.
//

#include <benchmark/benchmark.h>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <absl/strings/str_split.h>
#include <boost/algorithm/string.hpp>

void StringSplit(const std::string &str, const std::string &delimiters, std::vector<std::string> &elems) {
    std::string::size_type pos, prev = 0;
    while ((pos = str.find_first_of(delimiters, prev)) != std::string::npos) {
        if (pos > prev) {
            elems.emplace_back(str, prev, pos - prev);
        }
        prev = pos + 1;

    }
    if (prev < str.size())
        elems.emplace_back(str, prev, str.size() - prev);
}

auto StringSplitStream(const std::string &str, const char &delim) {
    thread_local auto ss=std::make_unique<std::istringstream>();
    ss->clear();
    ss->str(str);
    std::string token;
    std::vector<std::string> ret;
    while (std::getline(*ss, token, delim)) {
        ret.push_back(token);
    }
    return ret;
}

static void BenchStringSplit(benchmark::State &state) {
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r = 0;
    for (auto _ : state) {
        std::vector<std::string> vec;
        StringSplit(tag, ",", vec);
        for (auto &i : vec) {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchStringSplit)->ThreadRange(1, 8);

static void BenchStringSplitStream(benchmark::State &state) {
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r = 0;
    for (auto _ : state) {
        auto vec = StringSplitStream(tag, ',');
        for (auto &i : vec) {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchStringSplitStream)->ThreadRange(1, 8);

static void BenchBoostSplit(benchmark::State &state) {
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r = 0;
    for (auto _ : state) {
        std::vector<std::string> vec;
        boost::split(vec, tag, [](char c) { return c = ','; });
        for (auto &i : vec) {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchBoostSplit)->ThreadRange(1, 8);

static void BenchABSplit(benchmark::State &state) {
    std::string tag = "1,2,3,4,5,6,7,8,9,0,1,2,3,4,6,1,2,3,4,56,7,8,9,09,12";
    int r = 0;
    for (auto _ : state) {
        auto vec = absl::StrSplit(tag, ",");
        for (auto i : vec) {
            r += i.size();
        }
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchABSplit)->ThreadRange(1, 8);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}