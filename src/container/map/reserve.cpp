#include<benchmark/benchmark.h>
#include<unordered_map>
static void BM_insert(benchmark::State &state) {
    for (auto _ : state) {
        std::unordered_map<int, int> m;
        for (auto i = 0; i < 1024; i++) {
            m[i] = i;
        }
    }
}

static void BM_reserve(benchmark::State &state) {
    for (auto _ : state) {
        std::unordered_map<int, int> m;
        m.reserve(1024);
        for (auto i = 0; i < 1024; i++) {
            m[i] = i;
        }
    }
}


BENCHMARK(BM_insert);
BENCHMARK(BM_reserve);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}