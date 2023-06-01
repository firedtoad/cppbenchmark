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

#include "SortedVector.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
struct Pod
{
    uint64_t i;
    uint64_t c[1024];
    Pod() noexcept = default;
    explicit Pod(uint64_t i_) noexcept : i(i_) {}
    Pod(const Pod &pod) noexcept            = default;
    Pod(Pod &&pod) noexcept                 = default;
    Pod &operator=(const Pod &pod) noexcept = default;
    Pod &operator=(Pod &&pod) noexcept      = default;
};

struct NonPod
{
    uint64_t i        = 0;
    uint64_t c[1024];
    NonPod() noexcept = default;
    explicit NonPod(uint64_t i_) noexcept : i(i_) {}
    NonPod(const NonPod &p) noexcept            = default;
    NonPod(NonPod &&p) noexcept                 = default;
    NonPod &operator=(const NonPod &p) noexcept = default;
    NonPod &operator=(NonPod &&p) noexcept      = default;
};

struct TriviallyCopy
{
    uint64_t i;
    uint64_t c[1024];
};

struct NoTriviallyCopy
{
    uint64_t i;
    uint64_t c[1024];
    ~NoTriviallyCopy() {}
};

template <typename V> static void BenchInsertPod(benchmark::State &state)
{

    for (auto _ : state)
    {
        V v;
        v.reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}

BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<NonPod>)->Range(2, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<Pod>)->Range(2, 1 << 16);

BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<TriviallyCopy>)->Range(2, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<NoTriviallyCopy>)->Range(2, 1 << 16);

BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<std::pair<int, TriviallyCopy>>)->Range(2, 1 << 16);
BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<std::pair<const int, TriviallyCopy>>)->Range(2, 1 << 16);

template <typename P> static void BenchUninitializedCopy(benchmark::State &state)
{
    std::vector<P> pod(state.range(0));
    std::vector<P> dest(state.range(0));
    for (auto _ : state)
    {
        std::uninitialized_copy(pod.begin(), pod.end(), dest.begin());
        benchmark::DoNotOptimize(dest);
    }
}

BENCHMARK_TEMPLATE(BenchUninitializedCopy, NonPod)->Range(2, 1 << 16);
BENCHMARK_TEMPLATE(BenchUninitializedCopy, Pod)->Range(2, 1 << 16);

BENCHMARK_TEMPLATE(BenchUninitializedCopy, TriviallyCopy)->Range(2, 1 << 16);
BENCHMARK_TEMPLATE(BenchUninitializedCopy, NoTriviallyCopy)->Range(2, 1 << 16);

BENCHMARK_TEMPLATE(BenchUninitializedCopy, std::pair<int, TriviallyCopy>)->Range(2, 1 << 16);
BENCHMARK_TEMPLATE(BenchUninitializedCopy, std::pair<const int, TriviallyCopy>)->Range(2, 1 << 16);

int main(int argc, char **argv)
{
    std::cout << std::is_pod_v<Pod> << '\n';
    std::cout << std::is_pod_v<NonPod> << '\n';
    std::cout << std::is_standard_layout_v<Pod> << '\n';
    std::cout << std::is_standard_layout_v<NonPod> << '\n';
    std::cout << std::is_trivially_copyable_v<Pod> << '\n';
    std::cout << std::is_trivially_copyable_v<NonPod> << '\n';
    std::cout << std::is_trivially_copyable_v<TriviallyCopy> << '\n';
    std::cout << std::is_trivially_copyable_v<NoTriviallyCopy> << '\n';
    std::cout << std::is_trivially_copyable_v<std::pair<int, TriviallyCopy>> << '\n';
    std::cout << std::is_trivially_copyable_v<std::pair<const int, TriviallyCopy>> << '\n';
    std::cout << std::is_move_assignable_v<std::pair<int, TriviallyCopy>> << '\n';
    std::cout << std::is_move_assignable_v<std::pair<const int, TriviallyCopy>> << '\n';
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}