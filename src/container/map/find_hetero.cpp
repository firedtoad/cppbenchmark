#include <iostream>
#include <benchmark/benchmark.h>
#include <map>


static inline uint64_t xor_shift96() { /* A George Marsaglia generator, period 2^96-1 */
    static uint64_t x = 123456789, y = 362436069, z = 521288629;
    uint64_t t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline uint64_t _random() {
    return xor_shift96();
}

struct string_cmp {
    using is_transparent = int;
    template<class _Ty1, class _Ty2>
    constexpr auto operator()(_Ty1 &&_Left, _Ty2 &&_Right) const {
        return static_cast<_Ty1 && >(_Left) < static_cast<_Ty2 &&>(_Right);
    }
};

template<class M>
static void BenchOrderMapStringSSO(benchmark::State &state) {
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++) {
        keys[i] = std::to_string(i % 65536);
        m[keys[i]] = i;
    }
    for (auto _ : state) {
        auto kIndex = _random() % 65536;
        auto c = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapStringSSO, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapStringSSO, std::map<std::string, int, string_cmp>);

template<class M>
static void BenchOrderMapStringNoSSO(benchmark::State &state) {
    M m;
    std::vector<std::string> keys(65536);
    for (auto i = 0; i < 65536; i++) {
        keys[i] = "12345678901234561234567890123456" + std::to_string(i % 65536);
        m[keys[i]] = i;
    }

    for (auto _ : state) {
        auto kIndex = _random() % 65536;
        auto c = m.find(keys[kIndex].c_str());
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_TEMPLATE(BenchOrderMapStringNoSSO, std::map<std::string, int>);
BENCHMARK_TEMPLATE(BenchOrderMapStringNoSSO, std::map<std::string, int, string_cmp>);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
