// Copyright 2023 The Division Authors.
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

#include "utils/symbol.h"
#include <any>
#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>
#include <optional>
#include <utils/rss.h>
#include <variant>
#include <vector>

struct Point3D
{
    float x, y, z;
};

template <typename... T> void PrintList(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(t) << '\n'));
    std::cout << '\n';
}

template <class T> struct is_unique_ptr : std::false_type
{
};

template <class T, class D> struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type
{
};

template <class T> struct is_shared_ptr : std::false_type
{
};

template <class T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
{
};

template <typename T> struct Wrapper
{
    Wrapper() {}
    Wrapper(const T &v)
    {
        val.reset(new T{v});
    }
    Wrapper(const Wrapper &rhs)
    {
        val = std::make_unique<T>(*rhs.val.get());
    }
    Wrapper(Wrapper &&rhs)                 = default;
    Wrapper &operator=(const Wrapper &rhs) = default;
    Wrapper &operator=(Wrapper &&rhs)      = default;

    operator T &()
    {
        if (val)
        {
            return *val.get();
        }
        static T e{};
        return e;
    }

  private:
    std::unique_ptr<T> val{nullptr};
};

static std::string s = "1234567890123456";
template <typename T> static void BM_Variant(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::variant<int, double, T> va;
        if constexpr (is_unique_ptr<T>::value)
        {
            va = std::make_unique<std::string>(s);
        }
        else if constexpr (is_shared_ptr<T>::value)
        {
            va = std::make_shared<std::string>(s);
        }
        else if constexpr (std::is_pod_v<T>)
        {
            va = state.threads();
        }
        else
        {
            va = s;
        }
        benchmark::DoNotOptimize(va);
    }
}

static void BM_Any(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::any va;
        va = s;
        benchmark::DoNotOptimize(va);
    }
}

static void BM_AnyPod(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::any va;
        va = state.threads();
        benchmark::DoNotOptimize(va);
    }
}

static void BM_Optional(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::optional<std::string> va;
        va = s;
        benchmark::DoNotOptimize(va);
    }
}

static void BM_CopyableUniquePtrInt(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::variant<int, Wrapper<std::string>> va;
        va = state.threads();
        benchmark::DoNotOptimize(va);
    }
}

static void BM_CopyableUniquePtrString(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::variant<int, Wrapper<std::string>> va;
        va = s;
        benchmark::DoNotOptimize(va);
    }
}

BENCHMARK_TEMPLATE(BM_Variant, std::string);
BENCHMARK_TEMPLATE(BM_Variant, std::unique_ptr<std::string>);
BENCHMARK_TEMPLATE(BM_Variant, std::shared_ptr<std::string>);
BENCHMARK_TEMPLATE(BM_Variant, std::optional<std::string>);
BENCHMARK_TEMPLATE(BM_Variant, Wrapper<std::string>);

BENCHMARK_TEMPLATE(BM_Variant, int64_t);
BENCHMARK(BM_Any);
BENCHMARK(BM_AnyPod);
BENCHMARK(BM_Optional);
BENCHMARK(BM_CopyableUniquePtrInt);
BENCHMARK(BM_CopyableUniquePtrString);

int main(int argc, char **argv)
{
    {

        std::variant<int, double, Wrapper<std::string>> vau;
        //        auto b=vau;
        std::variant<int, double, std::shared_ptr<std::string>> vas;
        std::variant<int, double, std::string, bool> va;
        std::variant<int, int, short> vb;
        vb.index();
        std::tuple<int, double, std::string> tp;
        std::any any;
        std::optional<int64_t> op_i64;
        std::optional<std::string> op_str;
        PrintList(vau, vas, va, vb, tp, any, op_i64, op_str, std::unique_ptr<std::string>{});
    }
    {
        rusage rUsage{};
        FillRSS(rUsage);
        std::vector<std::variant<int, double, std::string, bool>> vec(1024 * 1024);
        PrintUsage(rUsage);
        FillRSS(rUsage);
        std::vector<std::variant<int, double, Wrapper<std::string>, bool>> vec1(1024 * 1024);
        static_assert(std::is_constructible_v<const std::variant<int, double, Wrapper<std::string>, bool>>);
        //        auto vec2 = vec1;
        PrintUsage(rUsage);
    }
    {
        Wrapper<std::string> ca(s);
        Wrapper<std::string> cd;
        auto cb = ca;
        auto cx = std::move(ca);
        std::cout << (std::string)ca << '\n' << (std::string)cb << '\n' << (std::string)cd << '\n' << (std::string)cx << '\n';
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}