#include <benchmark/benchmark.h>
#include <random>
#include <unordered_map>
#include <absl/random/random.h>
#include <boost/random.hpp>
#include "pcg_random.hpp"
#include "pcg_uint128.hpp"

template<typename G>
static void BenchEngine(benchmark::State &state) {
    for (auto _ : state) {
        G gen{};
        benchmark::DoNotOptimize(gen());
    }
}

BENCHMARK_TEMPLATE(BenchEngine, std::ranlux24_base);
BENCHMARK_TEMPLATE(BenchEngine, std::ranlux48_base);
BENCHMARK_TEMPLATE(BenchEngine, std::ranlux24);
BENCHMARK_TEMPLATE(BenchEngine, std::ranlux48);
BENCHMARK_TEMPLATE(BenchEngine, std::knuth_b);
BENCHMARK_TEMPLATE(BenchEngine, std::minstd_rand0);
BENCHMARK_TEMPLATE(BenchEngine, std::minstd_rand);
BENCHMARK_TEMPLATE(BenchEngine, std::mt19937);
BENCHMARK_TEMPLATE(BenchEngine, std::mt19937_64);

BENCHMARK_TEMPLATE(BenchEngine, boost::minstd_rand0);
BENCHMARK_TEMPLATE(BenchEngine, boost::minstd_rand);
BENCHMARK_TEMPLATE(BenchEngine, boost::rand48);
BENCHMARK_TEMPLATE(BenchEngine, boost::mt19937);
BENCHMARK_TEMPLATE(BenchEngine, boost::mt19937_64);
BENCHMARK_TEMPLATE(BenchEngine, boost::mt11213b);

template<typename P>
static void BenchPCG(benchmark::State &state) {

    for (auto _ : state) {
        P p;
        //    p.seed(pcg_extras::seed_seq_from<std::random_device>());
        benchmark::DoNotOptimize(p());
    }
}

BENCHMARK_TEMPLATE(BenchPCG, pcg32);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_oneseq);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_unique);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg64);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_oneseq);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_unique);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg8_once_insecure);
BENCHMARK_TEMPLATE(BenchPCG, pcg16_once_insecure);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_once_insecure);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_once_insecure);

BENCHMARK_TEMPLATE(BenchPCG, pcg8_oneseq_once_insecure);
BENCHMARK_TEMPLATE(BenchPCG, pcg16_oneseq_once_insecure);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_oneseq_once_insecure);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_oneseq_once_insecure);

BENCHMARK_TEMPLATE(BenchPCG, pcg32_k2);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_k2_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg32_k64);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_k64_oneseq);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_k64_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg32_c64);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_c64_oneseq);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_c64_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg64_k32);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_k32_oneseq);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_k32_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg64_c32);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_c32_oneseq);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_c32_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg32_k1024);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_k1024_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg32_c1024);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_c1024_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg64_k1024);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_k1024_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg64_c1024);
BENCHMARK_TEMPLATE(BenchPCG, pcg64_c1024_fast);

BENCHMARK_TEMPLATE(BenchPCG, pcg32_k16384);
BENCHMARK_TEMPLATE(BenchPCG, pcg32_k16384_fast);

template<typename P>
static void BenchABEngine(benchmark::State &state) {
    P pcg;
    for (auto _ : state) {
        benchmark::DoNotOptimize(pcg());
    }
}

BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::pcg32_2018_engine);
BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::pcg64_2018_engine);

BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::randen_engine<uint32_t>);
BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::randen_engine<uint64_t>);
BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::RandenPool<uint64_t>);

BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::NonsecureURBGBase<absl::random_internal::pcg32_2018_engine>);
BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::NonsecureURBGBase<absl::random_internal::pcg64_2018_engine>);

BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::NonsecureURBGBase<absl::random_internal::randen_engine<uint32_t>>);
BENCHMARK_TEMPLATE(BenchABEngine, absl::random_internal::NonsecureURBGBase<absl::random_internal::randen_engine<uint64_t>>);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}