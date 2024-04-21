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

#include "SFMT/SFMT.h"
#include <benchmark/benchmark.h>
#include <ext/random>
#include <functional>
#include <random>
#include <unordered_map>

template <typename G> static void BenchEngine(benchmark::State &state)
{
    static G mt{std::random_device{}()};
    for (auto _ : state)
    {
        for(auto i=0;i<1000;i++)
        {
            auto r=mt();
            benchmark::DoNotOptimize(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchEngine, std::mt19937);
BENCHMARK_TEMPLATE(BenchEngine, std::mt19937_64);
BENCHMARK_TEMPLATE(BenchEngine, std::minstd_rand);
BENCHMARK_TEMPLATE(BenchEngine, std::default_random_engine);

BENCHMARK_TEMPLATE(BenchEngine, __gnu_cxx::sfmt19937);
BENCHMARK_TEMPLATE(BenchEngine, __gnu_cxx::sfmt19937_64);

static void BenchSFMT(benchmark::State &state)
{

    sfmt_t _state;
    static std::random_device dev;
    //    if (dev.entropy() > 0) {
    std::array<uint32_t, SFMT_N32> seed;
    std::generate(seed.begin(), seed.end(), std::ref(dev));

    sfmt_init_by_array(&_state, seed.data(), seed.size());
    //    } else {
    //        sfmt_init_gen_rand(&_state, uint32_t(dev()));
    //    }
    for (auto _ : state)
    {
        for(auto i=0;i<1000;i++){
            auto r=sfmt_genrand_uint64(&_state);
            benchmark::DoNotOptimize(r);
        }
    }
}

BENCHMARK(BenchSFMT);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}