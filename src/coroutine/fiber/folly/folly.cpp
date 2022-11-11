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
#include <folly/fibers/FiberManagerMap.h>
#include <folly/io/async/EventBase.h>

#include <iostream>

static void BenchFiberCreatePost(benchmark::State &state)
{
    folly::EventBase evb;
    auto &fiberManager = folly::fibers::getFiberManager(evb);
    for (auto _ : state)
    {
        fiberManager.addTask([&] {});
    }
    evb.loop();
}

BENCHMARK(BenchFiberCreatePost);
static void BenchFiberCreateEager(benchmark::State &state)
{
    folly::EventBase evb;
    auto &fiberManager = folly::fibers::getFiberManager(evb);
    for (auto _ : state)
    {
        fiberManager.addTaskEager([&] {});
    }
    evb.loop();
}

BENCHMARK(BenchFiberCreateEager);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
