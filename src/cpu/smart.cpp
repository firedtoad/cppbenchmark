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
#include <deque>
#include <memory>
#include <unordered_map>
static void BenchPtr(benchmark::State &state)
{
    auto *p = new int{};
    for (auto _ : state)
    {
        if (p)
        {
            benchmark::DoNotOptimize(p);
        }
    }
}
BENCHMARK(BenchPtr);

static void BenchShared(benchmark::State &state)
{

    auto p = std::make_shared<int>();
    for (auto _ : state)
    {
        if (p)
        {
            benchmark::DoNotOptimize(p);
        }
    }
}

BENCHMARK(BenchShared);

void passVal(std::shared_ptr<int> p) { benchmark::DoNotOptimize(p); }
void passRef(std::shared_ptr<int> &r) { benchmark::DoNotOptimize(r); }

static void BenchPassVal(benchmark::State &state) {
    auto p = std::make_shared<int>();
    for (auto _ : state) {
        passVal(p);
    }
}
BENCHMARK(BenchPassVal);

static void BenchPassRef(benchmark::State &state) {
    auto p = std::make_shared<int>();
    for (auto _ : state) {
        passRef(p);
    }
}
BENCHMARK(BenchPassRef);
struct S{
    int w;
    int s;
};
__attribute__((noinline))
void passVal(S s){
    benchmark::DoNotOptimize(s);
}
__attribute__((noinline))
void passRef(S &s)
{
    benchmark::DoNotOptimize(s);
}
__attribute__((noinline))
void passAtomic(S &s)
{
    benchmark::DoNotOptimize(s);
}

static void BenchPassValS(benchmark::State &state) {
    S s;
    for (auto _ : state) {
        passVal(s);
    }
}
BENCHMARK(BenchPassValS);

static void BenchPassRefS(benchmark::State &state) {
    S s;
    for (auto _ : state) {
        passRef(s);
    }
}

BENCHMARK(BenchPassRefS);

static void BenchPassAtomic(benchmark::State &state) {
    S s;
    for (auto _ : state) {
        __atomic_fetch_add(&s.s, 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_add(&s.w, 1, __ATOMIC_ACQ_REL);
        passRef(s);
    }
}

BENCHMARK(BenchPassAtomic);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}