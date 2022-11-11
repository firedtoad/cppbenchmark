/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/
#ifndef __ASTAR_HPP__
#define __ASTAR_HPP__

#include "parallel_hashmap/phmap.h"
#include <functional>
#include <vector>
namespace AStar
{
struct Vec2i
{
    int x, y;

    bool operator==(const Vec2i &coordinates_) const;
};

using uint = unsigned int;
//    using HeuristicFunction = std::function<uint(Vec2i, Vec2i)>;
using HeuristicFunction = uint (*)(const Vec2i &, const Vec2i &);
//    using CollisionFunction = bool (*)(const Vec2i&);
using CollisionFunction = std::function<bool(const Vec2i &)>;
using CoordinateList    = std::vector<Vec2i>;

struct Node
{
    uint G = 0, H = 0;
    Vec2i coordinates;
    Node *parent = nullptr;

    Node(const Vec2i &coord_, Node *parent_ = nullptr);
    Node() = default;
    uint getScore() const;
};

struct CoordHash
{
    size_t operator()(const Vec2i &coord) const
    {
        return coord.x * 100000 + coord.y;
    }
};
auto comp      = [](const Node &pNode1, const Node &pNode2) { return pNode1.getScore() > pNode2.getScore(); };
using NodeHeap = std::vector<Node>;
//    using CoordMap = std::unordered_map<Vec2i,Node,CoordHash>;
// ska::flat_hash_map will revalid iterator when insert
using CoordMap = phmap::flat_hash_map<Vec2i, Node *, CoordHash>;
using CordSet  = phmap::flat_hash_set<Vec2i, CoordHash>;
class Generator
{
    bool detectCollision(const Vec2i &coordinates_);
    Node *findNodeOnMap(CoordMap &nodes_, const Vec2i &coordinates_);

  public:
    Generator();
    void setWorldSize(const Vec2i &worldSize_);
    void setDiagonalMovement(bool enable_);
    void setHeuristic(HeuristicFunction heuristic_);
    template <typename F> void SetCollision(F &&collison_)
    {
        collision = collison_;
    }
    CoordinateList findPath(const Vec2i &source_, const Vec2i &target_);
    void addCollision(const Vec2i &coordinates_);
    void removeCollision(const Vec2i &coordinates_);
    void clearCollisions();

  private:
    HeuristicFunction heuristic;
    CollisionFunction collision;
    CoordinateList direction, walls;
    Vec2i worldSize;
    uint directions;
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

#endif // __ASTAR_HPP__
