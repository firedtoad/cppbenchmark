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

#include "pcg_extras.hpp"
#include "pcg_random.hpp"
#include "pcg_uint128.hpp"
#include "pcg_variants.h"
#include <absl/random/random.h>
#include <benchmark/benchmark.h>
#include <bitset>
#include <boost/random.hpp>
#include <functional>
#include <random>
#include <unordered_map>

static void BenchCRandom(benchmark::State &state)
{
    srand(std::random_device{}());
    for (auto _ : state)
    {
        auto r=rand();
        benchmark::DoNotOptimize(r);
    }
}

template <typename G, typename D> static void BenchStdRandom(benchmark::State &state)
{
    G gen{std::random_device{}()};
    for (auto _ : state)
    {
        D dis(INT_MIN, INT_MAX);
        auto r = dis(gen);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BenchCRandom);

BENCHMARK_TEMPLATE(BenchStdRandom, std::minstd_rand0, std::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::minstd_rand0, std::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::minstd_rand0, std::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchStdRandom, std::minstd_rand, std::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::minstd_rand, std::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::minstd_rand, std::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchStdRandom, std::mt19937, std::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::mt19937, std::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::mt19937, std::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchStdRandom, std::mt19937_64, std::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::mt19937_64, std::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, std::mt19937_64, std::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchStdRandom, pcg32_fast, std::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, pcg32_fast, std::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, pcg32_fast, std::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchStdRandom, pcg64_fast, std::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, pcg64_fast, std::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchStdRandom, pcg64_fast, std::normal_distribution<>);

template <typename G, typename D> static void BenchBoostRandom(benchmark::State &state)
{
    G gen{std::random_device{}()};
    for (auto _ : state)
    {
        D dis(INT_MIN, INT_MAX);
        auto r=dis(gen);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostRandom, boost::minstd_rand0, boost::random::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::minstd_rand0, boost::random::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::minstd_rand0, boost::random::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchBoostRandom, boost::minstd_rand, boost::random::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::minstd_rand, boost::random::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::minstd_rand, boost::random::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt19937, boost::random::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt19937, boost::random::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt19937, boost::random::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt19937_64, boost::random::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt19937_64, boost::random::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt19937_64, boost::random::normal_distribution<>);

BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt11213b, boost::random::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt11213b, boost::random::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchBoostRandom, boost::mt11213b, boost::random::normal_distribution<>);

template <typename P, typename D> static void BenchAbRandom(benchmark::State &state)
{
    P p{std::random_device{}()};
    for (auto _ : state)
    {
        D dis(INT_MIN, INT_MAX);
        auto r=dis(p);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::pcg32_2018_engine, absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::pcg32_2018_engine, absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::pcg32_2018_engine, absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::pcg64_2018_engine, absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::pcg64_2018_engine, absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::pcg64_2018_engine, absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::randen_engine<uint32_t>, absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::randen_engine<uint32_t>, absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::randen_engine<uint32_t>, absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::randen_engine<uint64_t>, absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::randen_engine<uint64_t>, absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, absl::random_internal::randen_engine<uint64_t>, absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom, pcg32_fast, absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, pcg32_fast, absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, pcg32_fast, absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom, pcg64_fast, absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, pcg64_fast, absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom, pcg64_fast, absl::log_uniform_int_distribution<>);

int main(int argc, char **argv)
{
    //    pcg_engines::setseq_rxs_m_xs_16_16 ss;
    //    ss.seed(time(nullptr));
    //    {
    //
    //        pcg16i_random_t rng;
    //        pcg16i_srandom_r(&rng, 0, 0);
    //        std::cout << rng.state << " " << rng.inc << '\n';
    //        pcg16i_advance_r(&rng, 65535);
    //        std::cout << rng.state << " " << rng.inc << '\n';
    //        std::cout << "0 " << pcg16i_random_r(&rng) << '\n';
    //        std::cout << "1 " << pcg16i_random_r(&rng) << '\n';
    //        std::cout << "0 " << pcg16i_random_r(&rng) << '\n';
    //        std::cout << "1 " << pcg16i_random_r(&rng) << '\n';
    //        std::cout << "1 " << pcg16i_random_r(&rng) << '\n';
    //        std::cout << "1 " << pcg16i_random_r(&rng) << '\n';
    //        std::cout << "1 " << pcg16i_random_r(&rng) << '\n';
    //        std::unordered_map<uint16_t, uint16_t> mp;
    //        for (auto i{0}; i < 100000; i++)
    //        {
    //            auto s = pcg16i_random_r(&rng);
    //            if (mp[s]++ == 1)
    //            {
    //                std::cout << i << ' ' << s << '\n';
    //                break;
    //            }
    //        }
    //    }

    //    {
    //        pcg32i_random_t rng;
    //        pcg32i_srandom_r(&rng, 0, 0);
    //        std::vector<uint8_t> bset(size_t(UINT32_MAX) + 100, 0);
    //        for (size_t i{0}; i < size_t(UINT32_MAX) + 100; i++)
    //        {
    //            auto s = pcg32i_random_r(&rng);
    //            if (bset[s])
    //            {
    //                std::cout << i << ' ' << s << '\n';
    //                break;
    //            }
    //            bset[s] = 1;
    //        }
    //    }
    //    {
    //        std::linear_congruential_engine<uint32_t, 75, 74, 65537> lce;
    //        lce.seed(123456);
    //        std::unordered_map<uint16_t, uint16_t> mp;
    //        for (auto i{0}; i < 100000; i++)
    //        {
    //            auto s = lce();
    //            if (mp[s]++ == 1)
    //            {
    //                std::cout << i << ' ' << s << '\n';
    //                break;
    //            }
    //        }
    //    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}