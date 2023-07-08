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

#include "report.pb.h"
#include "utils/rss.h"
#include <benchmark/benchmark.h>
#include <experimental/unordered_map>
#include <vector>

static void BenchAdd(benchmark::State &state)
{
    for (auto _ : state)
    {
        pb_report::Ground report;
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}
BENCHMARK(BenchAdd)->Range(1, 65536);

static void BenchAddArena(benchmark::State &state)
{
    for (auto _ : state)
    {
        google::protobuf::Arena arena;
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}
BENCHMARK(BenchAddArena)->Range(1, 65536);

static void BenchAddArenaOptions(benchmark::State &state)
{
    for (auto _ : state)
    {
        google::protobuf::ArenaOptions options;
        options.start_block_size = 4096;
        options.max_block_size   = 1024 * 1024;
        google::protobuf::Arena arena(options);
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}
BENCHMARK(BenchAddArenaOptions)->Range(1, 65536);

static void BenchReserve(benchmark::State &state)
{

    for (auto _ : state)
    {
        pb_report::Ground report;
        report.mutable_attackerform()->Reserve(state.range(0));
        report.mutable_defenderform()->Reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}

BENCHMARK(BenchReserve)->Range(1, 65536);

static void BenchReserveArena(benchmark::State &state)
{
    for (auto _ : state)
    {
        google::protobuf::Arena arena;
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        report.mutable_attackerform()->Reserve(state.range(0));
        report.mutable_defenderform()->Reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}

BENCHMARK(BenchReserveArena)->Range(1, 65536);

const int SIZE = 65536;
int main(int argc, char **argv)
{
    rusage rUsage;
    FillRSS(rUsage);
    pb_report::Ground report;
    for (auto i = 0; i < SIZE; i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    benchmark::DoNotOptimize(report);
    PrintUsage(rUsage);
    FillRSS(rUsage);

    pb_report::Ground report1;
    report1.mutable_attackerform()->Reserve(SIZE);
    report1.mutable_defenderform()->Reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        report1.add_attackerform()->add_units();
        report1.add_defenderform()->add_units();
    }
    benchmark::DoNotOptimize(report);
    PrintUsage(rUsage);
    {
        FillRSS(rUsage);
        char buff[1024 * 1024 * 4];
        google::protobuf::ArenaOptions options;
        options.start_block_size = 4096;
        options.max_block_size   = 1024 * 1024;
        google::protobuf::Arena arena(options);
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        report.mutable_attackerform()->Reserve(SIZE);
        report.mutable_defenderform()->Reserve(SIZE);
        //        for (auto i = 0; i < SIZE; i++)
        //        {
        //            report.add_attackerform()->add_units();
        //            report.add_defenderform()->add_units();
        //        }
        std::cout << arena.SpaceUsed() << ":" << arena.SpaceAllocated() << '\n';
        PrintUsage(rUsage);
    }
    std::unordered_map<int, int> mp = {
        {1, 2},
        {2, 2},
        {3, 2},
    };
    std::experimental::erase_if(mp, [](auto &&it) {
                                    return true;
                                });
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}