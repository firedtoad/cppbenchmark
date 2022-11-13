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
        benchmark::DoNotOptimize(report1.attackerform_size() + report1.defenderform_size());
    }
}
BENCHMARK(BenchCopy)->Range(1, 65536);

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
        benchmark::DoNotOptimize(report1.attackerform_size() + report1.defenderform_size());
    }
}

BENCHMARK(BenchAddAllocate)->Range(1, 65536);
const int SIZE = 65536;
int main(int argc, char **argv)
{
    pb_report::Ground report;
    for (auto i = 0; i < SIZE; i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    benchmark::DoNotOptimize(report.attackerform_size() + report.defenderform_size());
    rusage rUsage;
    {

        FillRSS(rUsage);
        pb_report::Ground report1 = report;
        benchmark::DoNotOptimize(report1.attackerform_size() + report1.defenderform_size());
        std::cout<<report1.attackerform_size() + report1.defenderform_size()<<'\n';
        std::cout<<report.attackerform_size() + report.defenderform_size()<<'\n';
        printUsage(rUsage);
    }
    {
        FillRSS(rUsage);
        pb_report::Ground report1 = report;
        report1.Clear();
        benchmark::DoNotOptimize(report1.attackerform_size() + report1.defenderform_size());
        std::cout<<report1.attackerform_size() + report1.defenderform_size()<<'\n';
        std::cout<<report.attackerform_size() + report.defenderform_size()<<'\n';
        printUsage(rUsage);
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
        benchmark::DoNotOptimize(report1.attackerform_size() + report1.defenderform_size());
        std::cout<<report1.attackerform_size() + report1.defenderform_size()<<'\n';
        std::cout<<report.attackerform_size() + report.defenderform_size()<<'\n';
        printUsage(rUsage);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}