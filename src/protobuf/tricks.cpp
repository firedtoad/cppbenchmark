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
#include <deque>
#include <experimental/unordered_map>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static void BenchCopy(benchmark::State &state)
{
    pb_report::Ground report;
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    for (auto _ : state)
    {
        pb_report::Ground report1 = report;
        benchmark::DoNotOptimize(report1);
    }
}

BENCHMARK(BenchCopy)->Range(1, 1024);

static void BenchCopyArena(benchmark::State &state)
{

    google::protobuf::Arena arena;
    pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    for (auto _ : state)
    {
        pb_report::Ground &report1 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(report.GetArena());
        report1                    = report;
        benchmark::DoNotOptimize(report1);
    }
}

BENCHMARK(BenchCopyArena)->Range(1, 1024);

static void BenchCopyArenaDifferent(benchmark::State &state)
{
    google::protobuf::Arena arena;
    google::protobuf::Arena arena1;
    pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    for (auto _ : state)
    {
        pb_report::Ground &report1 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena1);
        report1                    = report;
        benchmark::DoNotOptimize(report1);
    }
}

BENCHMARK(BenchCopyArenaDifferent)->Range(1, 1024);

static void BenchSwap(benchmark::State &state)
{
    pb_report::Ground report;
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    pb_report::Ground report1;
    for (auto _ : state)
    {
        report1.Swap(&report);
        benchmark::DoNotOptimize(report1);
    }
}

BENCHMARK(BenchSwap)->Range(1, 1024);

static void BenchSwapArena(benchmark::State &state)
{
    google::protobuf::Arena arena;
    pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    pb_report::Ground &report1 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(report.GetArena());
    for (auto _ : state)
    {
        report1.Swap(&report);
        benchmark::DoNotOptimize(report1);
    }
}

BENCHMARK(BenchSwapArena)->Range(1, 1024);

static void BenchSwapArenaDifferent(benchmark::State &state)
{
    google::protobuf::Arena arena;
    google::protobuf::Arena arena1;
    pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    pb_report::Ground &report1 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena1);
    for (auto _ : state)
    {

        report1.Swap(&report);
        benchmark::DoNotOptimize(report1);
    }
}

BENCHMARK(BenchSwapArenaDifferent)->Range(1, 1024);

static void BenchAddAllocate(benchmark::State &state)
{
    pb_report::Ground report;
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }

    for (auto _ : state)
    {
        pb_report::Ground report1;

        for (auto i = 0; i < state.range(0); i++)
        {
            report1.mutable_attackerform()->AddAllocated(report.mutable_attackerform(i));
            report1.mutable_defenderform()->AddAllocated(report.mutable_defenderform(i));
        }
        report1.mutable_attackerform()->ExtractSubrange(0, report1.attackerform_size(), nullptr);
        report1.mutable_defenderform()->ExtractSubrange(0, report1.defenderform_size(), nullptr);
        benchmark::DoNotOptimize(report1);
        report1.Clear();
    }
}
BENCHMARK(BenchAddAllocate)->Range(1, 1024);

static void BenchAddAllocateArena(benchmark::State &state)
{
    google::protobuf::Arena arena;
    pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
    for (auto i = 0; i < state.range(0); i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }

    for (auto _ : state)
    {
        pb_report::Ground &report1 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(report.GetArena());
        for (auto i = 0; i < state.range(0); i++)
        {
            report1.mutable_attackerform()->AddAllocated(report.mutable_attackerform(i));
            report1.mutable_defenderform()->AddAllocated(report.mutable_defenderform(i));
        }
        report1.mutable_attackerform()->ExtractSubrange(0, report1.attackerform_size(), nullptr);
        report1.mutable_defenderform()->ExtractSubrange(0, report1.defenderform_size(), nullptr);
        benchmark::DoNotOptimize(report1);
        report1.Clear();
    }
}

BENCHMARK(BenchAddAllocateArena)->Range(1, 1024);

template <typename SourceContainerType, typename T = typename SourceContainerType::value_type::first_type>
std::pair<bool, T> RandomByAddedRate(const SourceContainerType &vecSource)
{
    return {true, T{}};
}

const int SIZE = 65536;
int main(int argc, char **argv)
{
    pb_report::Ground report;
    for (auto i = 0; i < SIZE; i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    benchmark::DoNotOptimize(report);
    rusage rUsage;
    {

        FillRSS(rUsage);
        pb_report::Ground report1 = report;
        benchmark::DoNotOptimize(report1);
        std::cout << report1.attackerform_size() + report1.defenderform_size() << '\n';
        std::cout << report.attackerform_size() + report.defenderform_size() << '\n';
        PrintUsage(rUsage);
    }
    {
        FillRSS(rUsage);
        pb_report::Ground report1 = report;
        report1.Clear();
        benchmark::DoNotOptimize(report1);
        std::cout << report1.attackerform_size() + report1.defenderform_size() << '\n';
        std::cout << report.attackerform_size() + report.defenderform_size() << '\n';
        PrintUsage(rUsage);
    }
    {
        FillRSS(rUsage);
        pb_report::Ground report1;
        for (auto i = 0; i < SIZE; i++)
        {
            report1.mutable_attackerform()->AddAllocated(report.mutable_attackerform(i));
            report1.mutable_defenderform()->AddAllocated(report.mutable_defenderform(i));
        }
        report1.mutable_attackerform()->UnsafeArenaExtractSubrange(0, report1.attackerform_size(), nullptr);
        report1.mutable_defenderform()->UnsafeArenaExtractSubrange(0, report1.defenderform_size(), nullptr);
        benchmark::DoNotOptimize(report1);
        std::cout << report1.attackerform_size() + report1.defenderform_size() << '\n';
        std::cout << report.attackerform_size() + report.defenderform_size() << '\n';
        PrintUsage(rUsage);
    }
    {

        google::protobuf::ArenaOptions options;
        options.start_block_size = 8192;
        options.max_block_size   = 8192;
        google::protobuf::Arena xArena(options);
        pb_report::Ground &report1 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&xArena);
        report1.mutable_attackerform()->Reserve(SIZE);
        report1.mutable_defenderform()->Reserve(SIZE);
        for (auto i = 0; i < SIZE; i++)
        {
            report1.mutable_attackerform()->Add();
            report1.mutable_defenderform()->Add();
        }
        std::cout << xArena.SpaceAllocated() << ' ' << xArena.SpaceUsed() << '\n';
        auto strData = report1.SerializeAsString();
        {
            google::protobuf::ArenaOptions options;
            options.start_block_size = 8192;
            options.max_block_size   = 8192;
            google::protobuf::Arena xArena(options);

            pb_report::Ground &report2 = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&xArena);
            report2.mutable_attackerform()->Reserve(report1.attackerform_size());
            report2.mutable_defenderform()->Reserve(report1.defenderform_size());
            report2.ParseFromString(strData);
            std::cout << xArena.SpaceAllocated() << ' ' << xArena.SpaceUsed() << '\n';
            std::cout << report2.attackerform().size() << ' ' << report2.attackerform().Capacity() << '\n';
        }
    }

    //    std::cout<<sizeof(std::list<int>)<<'\n';
    //    std::cout<<sizeof(std::deque<int>)<<'\n';
    //    std::cout<<sizeof(std::vector<int>)<<'\n';
    //    std::cout<<sizeof(std::forward_list<int>)<<'\n';

    //        benchmark::Initialize(&argc, argv);
    //        benchmark::RunSpecifiedBenchmarks();
    return 0;
}