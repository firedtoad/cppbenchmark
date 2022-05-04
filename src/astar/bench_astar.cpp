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
const bool rand_pos = false;
static AStar::Generator generator;
static AStar::Vec2i start_pos = {0, 0};
static AStar::Vec2i end_pos = {WIDTH - 1, HEIGHT - 1};

void BenchmarkAstar(benchmark::State &state) {

    for (auto _ : state) {
        generator.findPath(start_pos, end_pos);
    }
    state.SetItemsProcessed(state.iterations() );
}
BENCHMARK(BenchmarkAstar);

inline std::ostream &operator<<(std::ostream &os, const AStar::Vec2i &coord_) {
    return os << coord_.x << "," << coord_.y;
}

int main(int argc, char **argv) {
    generator.setWorldSize({WIDTH, HEIGHT});
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<> dis(0, 10000);
    std::array<std::array<char, WIDTH>, HEIGHT> cc;
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

    if (rand_pos) {
        std::uniform_int_distribution<> dis(0, 500);
        start_pos = {dis(gen), dis(gen)};
        end_pos = {dis(gen), dis(gen)};
    }
    std::cout << start_pos <<" : "<< end_pos << '\n';
    do {
        c_gen();

        auto p = generator.findPath(start_pos, end_pos);
        if (!p.empty()) {
            break;
        }
    } while (1);
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}