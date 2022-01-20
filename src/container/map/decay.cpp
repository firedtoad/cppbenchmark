//
// Created by Administrator on 2022/01/15.
//

#include<benchmark/benchmark.h>
#include<unordered_map>

struct Hasher {
    size_t operator()(int key) const{
        return 1;
    }
};

std::unordered_map<int, int> unmap;
std::unordered_map<int, int, Hasher> decay_unmap;

static void BM_unmap(benchmark::State &state) {
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            unmap[i] = i;
        }
    }
}

static void BM_decay_map(benchmark::State &state) {
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            decay_unmap[i] = i;
        }
    }
}

BENCHMARK(BM_unmap);
BENCHMARK(BM_decay_map);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}