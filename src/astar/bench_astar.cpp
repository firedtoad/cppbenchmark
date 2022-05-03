//
// Created by Administrator on 2022/01/15.
//
#include "AStar.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <benchmark/benchmark.h>

const int WIDTH = 500;
const int HEIGHT = 500;

static AStar::Generator generator;

void BenchmarkAstar(benchmark::State &state) {
    for (auto _ : state) {
        generator.findPath({0, 0}, {WIDTH - 1, HEIGHT - 1});
    }
}
BENCHMARK(BenchmarkAstar);
int main(int argc, char **argv) {
    generator.setWorldSize({WIDTH, HEIGHT});
    std::random_device rd;
    std::default_random_engine gen{};
    std::uniform_int_distribution<> dis(0, 10000);
    std::array <std::array<char, WIDTH>, HEIGHT> cc;
    generator.SetCollision([&cc](const AStar::Vec2i &cord) {
        int x = cord.x;
        int y = cord.y;
        if (x < WIDTH && y < HEIGHT) {
            return cc[x][y] > 0;
        }
        return false;
    });

    generator.setHeuristic(AStar::Heuristic::octagonal);
    const int prob = 2000;
    auto c_gen = [&cc, &dis, &gen]() {
        cc.fill({});
        for (auto &it : cc) {
            for (auto &cit : it) {
                if (dis(gen) < prob) {
                    cit = 1;
                }
            }
        }
    };

    do {
        c_gen();
        auto p = generator.findPath({0, 0}, {WIDTH - 1, HEIGHT - 1});
        if (!p.empty()) {
            break;
        }
    } while (1);
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}