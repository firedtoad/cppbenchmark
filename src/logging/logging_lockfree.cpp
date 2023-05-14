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
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

static void BM_SpdLog(benchmark::State &state)
{
    static auto logger = spdlog::create<spdlog::sinks::rotating_file_sink_mt>("file_logger", "sync_log.txt", 1024 * 1024 * 128, 10);
    spdlog::set_default_logger(logger);
    auto i = 0, j = 0, k = 0;
    std::string ss = "adssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";
    for (auto _ : state)
    {
        SPDLOG_INFO("simple message {} {} {} {}", i++, j++, k++, ss);
    }
}
BENCHMARK(BM_SpdLog);
BENCHMARK(BM_SpdLog)->ThreadRange(1,8);

static void BM_SpdLogAsync(benchmark::State &state)
{
    static auto async_logger =
        spdlog::create_async_nb<spdlog::sinks::rotating_file_sink_mt>("async_file_logger", "async_log.txt", 1024 * 1024 * 128, 10);
    spdlog::set_default_logger(async_logger);
    auto i = 0, j = 0, k = 0;
    std::string ss = "adssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";
    for (auto _ : state)
    {
        SPDLOG_INFO("simple message {} {} {} {}", i++, j++, k++, ss);
    }
}

BENCHMARK(BM_SpdLogAsync);
BENCHMARK(BM_SpdLogAsync)->ThreadRange(1,8);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}