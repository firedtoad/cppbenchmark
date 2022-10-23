//
// Created by Administrator on 2022/01/15.
//
#include "AStar.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <random>
#include <thread>

const int WIDTH     = 500;
const int HEIGHT    = 500;
const bool rand_pos = false;
static AStar::Generator generator_;
static AStar::Vec2i start_pos = {0, 0};
static AStar::Vec2i end_pos   = {WIDTH - 1, HEIGHT - 1};

static void BenchmarkAStar(benchmark::State &state)
{
    for (auto _ : state)
    {
        generator_.findPath(start_pos, end_pos);
    }
    state.SetItemsProcessed(state.iterations());
}

//BENCHMARK(BenchmarkAStar);

//static void BM_Astar(benchmark::State& state) {
//    for (auto _ : state) {
//        //      auto path = generator.findPath({49, 76}, {87,70});
//        generator_.findPath({1,1}, {255,255});
//    }
//}

//BENCHMARK(BM_Astar);


inline std::ostream &operator<<(std::ostream &os, const AStar::Vec2i &coord_)
{
    return os << coord_.x << "," << coord_.y;
}

int main1(int argc, char **argv)
{
    generator_.setWorldSize({WIDTH, HEIGHT});
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<> dis(0, 10000);
    std::vector<std::vector<char>> cc(WIDTH);
    for (auto &it : cc)
    {
        it.resize(HEIGHT);
    }
    generator_.SetCollision(
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

    generator_.setHeuristic(AStar::Heuristic::manhattan);
    const int prob = 500;
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

        auto p = generator_.findPath(start_pos, end_pos);
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