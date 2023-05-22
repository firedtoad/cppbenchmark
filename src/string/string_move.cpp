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
#include <unordered_map>
struct LogCopy
{
    template <typename F>
    LogCopy(const std::string &file, int line, const std::string &fun, F &&log)
        : m_file(file), m_line(line), m_fun(fun), m_log(std::forward<F>(log)())
    {
    }
    std::string m_file;
    int m_line;
    std::string m_fun;
    std::string m_log;
};

struct LogMove
{
    template <typename F>
    LogMove(std::string file, int line, std::string fun, F &&log)
        : m_file(std::move(file)), m_line(line), m_fun(std::move(fun)), m_log(std::forward<F>(log)())
    {
    }
    std::string m_file;
    int m_line;
    std::string m_fun;
    std::string m_log;
};

template <typename S> static void BenchStringMove(benchmark::State &state)
{
    auto fn = [] { return "1111111111111111111111111111111111111111111111111111111111111111111111111111111"; };
    for (auto _ : state)
    {
        S s(__FILE__, __LINE__, __PRETTY_FUNCTION__, std::move(fn));
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_TEMPLATE(BenchStringMove, LogCopy);
BENCHMARK_TEMPLATE(BenchStringMove, LogMove);

struct Pod
{
    Pod() {}

    void ReSize(size_t sz)
    {
        s.resize(sz);
    }
    Pod(const Pod &) noexcept = default;
    //    Pod(Pod &&) noexcept = default ;
    //    Pod &operator=(Pod &&) noexcept = default ;
    void Swap(Pod &p)
    {
        s.swap(p.s);
    }
    ~Pod() {}
    std::string s;
};

Pod rvo()
{
    Pod p{};
    p.ReSize(1024);
    return p;
}

static void BenchRvo(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = rvo();
        benchmark::DoNotOptimize(r);
    }
}

static void BenchCopy(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::move(rvo());
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BenchCopy);
BENCHMARK(BenchRvo);

static void BenchUnmapCopy(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::unordered_map<uint32_t, Pod> umap;
        Pod p;
        p.ReSize(state.range(0));
        umap.insert(std::make_pair(1, p));
        benchmark::DoNotOptimize(umap);
    }
}

static void BenchUnmapRvo(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::unordered_map<uint32_t, Pod> umap;
        Pod p;
        p.ReSize(state.range(0));
        umap[1].Swap(p);
        benchmark::DoNotOptimize(umap);
    }
}
BENCHMARK(BenchUnmapCopy)->Range(1, 1 << 10);
BENCHMARK(BenchUnmapRvo)->Range(1, 1 << 10);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
