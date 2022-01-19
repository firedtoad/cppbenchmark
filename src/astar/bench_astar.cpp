//
// Created by Administrator on 2022/01/15.
//
#include "AStar.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <benchmark/benchmark.h>

static AStar::Generator generator;

void BenchmarkAstar(benchmark::State &state) {
    for (auto _ : state) {
        generator.findPath({1, 1}, {499, 499});
    }
}
BENCHMARK(BenchmarkAstar);
int main(int argc, char **argv) {
    generator.setWorldSize({500, 500});
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> dis(0, 10000);
    char cc[500][500] = {0};
    for (auto &it:cc) {
        for (auto &cit:it) {
            if (dis(gen) < 2000) {
                cit = 1;
            }
        }
    }
    generator.SetCollision([cc](const AStar::Vec2i &cord) {
        if (cord.x < 500 && cord.y < 500) {
            return cc[cord.x][cord.y] > 0;
        }
        return false;
    });

    generator.setHeuristic(AStar::Heuristic::octagonal);
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}