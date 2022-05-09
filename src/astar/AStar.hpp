/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/
#ifndef __ASTAR_HPP__
#define __ASTAR_HPP__

#include <vector>
#include <functional>
#include "tsl/robin_map.h"
#include "tsl/robin_set.h"

namespace AStar {

struct Vec2i {
    int32_t x, y;
    bool operator==(const Vec2i &coordinates_) const;
};
using uint = uint32_t;
using HeuristicFunction = uint (*)(const Vec2i &, const Vec2i &);
using RelaxFunction = float (*)(float e, int n, int len);
using CollisionFunction = std::function<bool(const Vec2i &)>;
using CoordinateList = std::vector<Vec2i>;

struct Node {
    uint G = 0, H = 0;
    uint S = 0;
    Vec2i coordinates;
    Node *parent = nullptr;

    Node(const Vec2i &coord_, Node *parent_ = nullptr);
    Node() = default;
    uint getScore() const;
    void updateScore();
};

struct CoordHash {
    size_t operator()(const Vec2i &coord) const {
        return (coord.x * 100000) + coord.y;
    }
};

using NodeHeap = std::vector<Node *>;
using CoordMap = tsl::robin_map<Vec2i, Node *, CoordHash>;
using CordSet  = tsl::robin_set<Vec2i, CoordHash>;
class Generator {
    bool detectCollision(const Vec2i &coordinates_);
    Node *findNodeOnMap(CoordMap &nodes_, const Vec2i &coordinates_);
public:
    Generator();
    void setWorldSize(const Vec2i &worldSize_);
    void setDiagonalMovement(bool enable_);
    void setWeight(float epsilon_);
    void setRelaxFunction(RelaxFunction relaxer_);
    void setHeuristic(HeuristicFunction heuristic_);
    template<typename F>
    void SetCollision(F &&collision_) {
        collision = std::forward<F>(collision_);
    }
    CoordinateList findPath(const Vec2i &source_, const Vec2i &target_);
    CoordinateList findPathStack(const Vec2i &source_, const Vec2i &target_);
private:
    HeuristicFunction heuristic;
    CollisionFunction collision;
    RelaxFunction relaxer;
    CoordinateList direction;
    Vec2i worldSize;
    float epsilon;
    uint directions;
};
class Relaxer {
public:
    static float dynamic_weight(float e, int n, int len);
    static float static_weight(float e, int n, int len);
};
class Heuristic {
public:
    static Vec2i getDelta(const Vec2i &source_, const Vec2i &target_);
    static uint manhattan(const Vec2i &source_, const Vec2i &target_);
    static uint euclidean(const Vec2i &source_, const Vec2i &target_);
    static uint octagonal(const Vec2i &source_, const Vec2i &target_);
};
}

#endif // __ASTAR_HPP__
