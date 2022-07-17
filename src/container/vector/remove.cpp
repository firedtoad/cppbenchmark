//
// Created by Administrator on 2022/01/15.
//

//
// Created by Administrator on 2021/9/23.
//

#include<benchmark/benchmark.h>
#include<vector>
#include<list>
#include<string>
#include<iostream>

constexpr int N = 1000;


void init(std::vector<std::string> &v1) {
    v1.resize(N);
    for (auto i = 0; i < N; i++) {
        v1[i] = std::to_string(i);
    }
}

static unsigned long xorshf96() { /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long random_() {
    return xorshf96();
}

void init(std::list<std::string> &v1) {
    for (auto i = 0; i < N; i++) {
        v1.push_back(std::to_string(i * 1000));
    }
}


static void BM_erase(benchmark::State &state) {
    std::vector<std::string> vec;
    init(vec);
    for (auto _ : state) {
        auto idx = random_() % vec.size();
        auto start = std::next(vec.begin(), idx);
        auto val = *start;
        vec.erase(start);
        vec.push_back(val);
    }
}

static void BM_swap_pop(benchmark::State &state) {
    std::vector<std::string> vec;
    init(vec);
    for (auto _ : state) {
        auto idx = random_() % vec.size();
        auto start = std::next(vec.begin(), idx);
        auto val = *start;
        std::swap(*start, vec.back());
        vec.pop_back();
        vec.push_back(val);
    }
}

static void BM_list(benchmark::State &state) {
    std::list<std::string> vec;
    init(vec);
    for (auto _ : state) {
        auto idx = random_() % vec.size();
        auto start = std::next(vec.begin(), idx);
        auto val = *start;
        vec.erase(start);
        vec.push_back(val);
    }
}


BENCHMARK(BM_erase);
BENCHMARK(BM_swap_pop);
BENCHMARK(BM_list);

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}