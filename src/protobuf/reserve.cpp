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

static void BenchAdd(benchmark::State &state)
{
    for (auto _ : state)
    {
        pb_report::Ground report;
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();;
            report.add_defenderform()->add_units();;
        }
        benchmark::DoNotOptimize(report.attackerform_size()+report.defenderform_size());
    }
}
BENCHMARK(BenchAdd)->Range(1, 65536);

static void BenchReserve(benchmark::State &state)
{

    for (auto _ : state)
    {
        pb_report::Ground report;
        report.mutable_attackerform()->Reserve(state.range(0));
        report.mutable_attackerform()->Reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();;
            report.add_defenderform()->add_units();;
        }
        benchmark::DoNotOptimize(report.attackerform_size()+report.defenderform_size());
    }
}

BENCHMARK(BenchReserve)->Range(1, 65536);

const int SIZE = 65536 ;
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
    benchmark::DoNotOptimize(report.attackerform_size()+report.defenderform_size());
    printUsage(rUsage);
    FillRSS(rUsage);

    pb_report::Ground report1;
    report1.mutable_attackerform()->Reserve(SIZE);
    report1.mutable_attackerform()->Reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        report1.add_attackerform()->add_units();;
        report1.add_defenderform()->add_units();;
    }
    benchmark::DoNotOptimize(report.attackerform_size()+report.defenderform_size());
    printUsage(rUsage);
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}