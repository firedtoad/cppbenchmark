#include <benchmark/benchmark.h>
#include <random>
#include <unordered_map>
#include "SFMT/SFMT.h"

template<typename G>
static void BenchEngine(benchmark::State &state) {
    G mt{std::random_device{}()};
    for (auto _ : state) {
        benchmark::DoNotOptimize(mt());
    }
}

BENCHMARK_TEMPLATE(BenchEngine, std::mt19937
);
BENCHMARK_TEMPLATE(BenchEngine, std::mt19937_64
);
BENCHMARK_TEMPLATE(BenchEngine, std::minstd_rand
);
BENCHMARK_TEMPLATE(BenchEngine, std::default_random_engine
);

static void BenchSFMT(benchmark::State &state) {

    sfmt_t _state;
    std::random_device dev;
//    if (dev.entropy() > 0) {
    std::array<uint32_t, SFMT_N32> seed;
    std::generate(seed.begin(), seed.end(), std::ref(dev));

    sfmt_init_by_array(&_state, seed.data(), seed.size());
//    } else {
//        sfmt_init_gen_rand(&_state, uint32_t(dev()));
//    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(sfmt_genrand_uint64(&_state));
    }
}

BENCHMARK(BenchSFMT);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}