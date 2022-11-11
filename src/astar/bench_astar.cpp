// Copyright 2020 The Division Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Author dietoad@gmail.com && firedtoad@gmail.com

#include "AStar.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <random>
#include <thread>

const int WIDTH     = 500;
const int HEIGHT    = 500;
const bool rand_pos = false;
static AStar::Generator generator;
static AStar::Vec2i start_pos = {0, 0};
static AStar::Vec2i end_pos   = {WIDTH - 1, HEIGHT - 1};

void BenchmarkAStar(benchmark::State &state)
{
    for (auto _ : state)
    {
        generator.findPath(start_pos, end_pos);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BenchmarkAStar);

void BenchmarkAStarStack(benchmark::State &state)
{

    for (auto _ : state)
    {
        generator.findPathStack(start_pos, end_pos);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BenchmarkAStarStack);

inline std::ostream &operator<<(std::ostream &os, const AStar::Vec2i &coord_)
{
    return os << coord_.x << "," << coord_.y;
}

int main(int argc, char **argv)
{
    generator.setWorldSize({WIDTH, HEIGHT});
    generator.setStep(1);
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<> dis(0, 10000);
    std::vector<std::vector<char>> cc(WIDTH);
    for (auto &it : cc)
    {
        it.resize(HEIGHT);
    }
    generator.SetCollision(
        [&cc](const AStar::Vec2i &cord)
        {
            int x = cord.x;
            int y = cord.y;
            if (x < WIDTH && y < HEIGHT)
            {
                return cc[x][y] > 0;
            }
            return false;
        });

    generator.setHeuristic(AStar::Heuristic::octagonal);
    const int prob = 2000;
    auto c_gen     = [&cc, &dis, &gen]()
    {
        for (auto &it : cc)
        {
            std::fill(it.begin(), it.end(), 0);
        }
        for (auto &it : cc)
        {
            for (auto &cit : it)
            {
                if (dis(gen) < prob)
                {
                    cit = 1;
                }
            }
        }
    };

    if (rand_pos)
    {
        std::uniform_int_distribution<> dis(0, 500);
        start_pos = {dis(gen), dis(gen)};
        end_pos   = {dis(gen), dis(gen)};
    }
    std::cout << start_pos << " : " << end_pos << '\n';
    do
    {
        c_gen();

        auto p = generator.findPath(start_pos, end_pos);
        if (!p.empty())
        {
            for (auto &it : p)
            {
                std::cout << it.x << "," << it.y << '\n';
            }
            break;
        }
    } while (1);
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}