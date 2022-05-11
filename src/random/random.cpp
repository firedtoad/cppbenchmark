#include <benchmark/benchmark.h>
#include <functional>
#include <random>
#include <unordered_map>
#include <absl/random/random.h>
#include <boost/random.hpp>
#include "pcg_random.hpp"
#include "pcg_uint128.hpp"
#include "pcg_extras.hpp"

static void BenchCRandom(benchmark::State &state) {
    srand(std::random_device{}());
    for (auto _ : state) {
        benchmark::DoNotOptimize(rand());
    }
}

template<typename G, typename D>
static void BenchStdRandom(benchmark::State &state) {
    G gen{std::random_device{}()};
    for (auto _ : state) {
        D dis(INT_MIN, INT_MAX);
        benchmark::DoNotOptimize(dis(gen));
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


template<typename G, typename D>
static void BenchBoostRandom(benchmark::State &state) {
    G gen{std::random_device{}()};
    for (auto _ : state) {
        D dis(INT_MIN, INT_MAX);
        benchmark::DoNotOptimize(dis(gen));
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

template<typename P, typename D>
static void BenchAbRandom(benchmark::State &state) {
    P p{std::random_device{}()};
    for (auto _ : state) {
        D dis(INT_MIN,INT_MAX);
        benchmark::DoNotOptimize(dis(p));
    }
}

BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::pcg32_2018_engine,absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::pcg32_2018_engine,absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::pcg32_2018_engine,absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::pcg64_2018_engine,absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::pcg64_2018_engine,absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::pcg64_2018_engine,absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::randen_engine<uint32_t>,absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::randen_engine<uint32_t>,absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::randen_engine<uint32_t>,absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::randen_engine<uint64_t>,absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::randen_engine<uint64_t>,absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,absl::random_internal::randen_engine<uint64_t>,absl::log_uniform_int_distribution<>);


BENCHMARK_TEMPLATE(BenchAbRandom,pcg32_fast,absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,pcg32_fast,absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,pcg32_fast,absl::log_uniform_int_distribution<>);

BENCHMARK_TEMPLATE(BenchAbRandom,pcg64_fast,absl::uniform_int_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,pcg64_fast,absl::uniform_real_distribution<>);
BENCHMARK_TEMPLATE(BenchAbRandom,pcg64_fast,absl::log_uniform_int_distribution<>);


int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}