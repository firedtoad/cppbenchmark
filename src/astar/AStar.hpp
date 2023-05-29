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

#ifndef A_STAR_HPP_H
#define A_STAR_HPP_H

#include "parallel_hashmap/phmap.h"
//#include "tsl/sparse_map.h"
//#include "tsl/sparse_set.h"
//#include <folly/container/F14Map.h>
//#include <folly/container/F14Set.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace AStar
{

struct Vec2i
{
    int32_t x, y;
    bool operator==(const Vec2i &coordinates_) const;
};
using uint              = uint32_t;
using HeuristicFunction = uint (*)(const Vec2i &, const Vec2i &);
using RelaxFunction     = float (*)(float e, int n, int len);
using CollisionFunction = std::function<bool(const Vec2i &)>;
using CoordinateList    = std::vector<Vec2i>;

struct Node
{
    uint G = 0, H = 0;
    uint S = 0;
    Vec2i coordinates;
    Node *parent = nullptr;

    Node(const Vec2i &coord_, Node *parent_ = nullptr);
    Node() = default;
    uint getScore() const;
    void updateScore();
};

struct CoordHash
{
    size_t operator()(const Vec2i &coord) const
    {
        //        return phmap::HashState::combine(31,coord.x,coord.y);
        return (size_t(coord.x) * 32) + coord.y;
    }
    static inline Vec2i getEmptyKey()
    {
        return {-1, -1};
    }
    static inline Vec2i getTombstoneKey()
    {
        return {-2, -2};
    }
    static unsigned getHashValue(const Vec2i &coord)
    {
        return (size_t(coord.x) * 31) + coord.y;
    }
    static bool isEqual(const Vec2i &LHS, const Vec2i &RHS)
    {
        return LHS == RHS;
    }
};

using NodeHeap = std::vector<Node *>;
// using CoordMap = std::unordered_map<Vec2i, Node *, CoordHash>;
// using CordSet  = std::unordered_set<Vec2i, CoordHash>;
 using CoordMap = phmap::flat_hash_map<Vec2i, Node *, CoordHash>;
 using CordSet  = phmap::flat_hash_set<Vec2i, CoordHash>;
// using CoordMap = tsl::sparse_map<Vec2i, Node *, CoordHash>;
// using CordSet  = tsl::sparse_set<Vec2i, CoordHash>;
//using CoordMap = folly::F14FastMap<Vec2i, Node *, CoordHash>;
//using CordSet  = folly::F14FastSet<Vec2i, CoordHash>;

//using CoordMap = llvm::DenseMap<Vec2i, Node *,CoordHash>;
//using CordSet  = phmap::flat_hash_set<Vec2i, CoordHash>;

class Generator
{
    bool detectCollision(const Vec2i &coordinates_);
    Node *findNodeOnMap(CoordMap &nodes_, const Vec2i &coordinates_);

  public:
    Generator();
    void setWorldSize(const Vec2i &worldSize_);
    void setDiagonalMovement(bool enable_);
    void setWeight(float epsilon_);
    void setStep(int32_t step_);
    void setRelaxFunction(RelaxFunction relaxer_);
    void setHeuristic(HeuristicFunction heuristic_);
    template <typename F> void SetCollision(F &&collision_)
    {
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
    int32_t step;
    uint directions;
};
class Relaxer
{
  public:
    static float dynamic_weight(float e, int n, int len);
    static float static_weight(float e, int n, int len);
};
class Heuristic
{
  public:
    static Vec2i getDelta(const Vec2i &source_, const Vec2i &target_);
    static uint manhattan(const Vec2i &source_, const Vec2i &target_);
    static uint euclidean(const Vec2i &source_, const Vec2i &target_);
    static uint octagonal(const Vec2i &source_, const Vec2i &target_);
};
} // namespace AStar

#endif // A_STAR_HPP_H
