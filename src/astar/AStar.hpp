/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/
#ifndef __ASTAR_HPP__
#define __ASTAR_HPP__

#include <vector>
#include <functional>
#include "flat_hash_map.hpp"
#include "unordered_map.hpp"
#include <unordered_map>
namespace AStar
{
    struct Vec2i
    {
        int x, y;

        bool operator==(const Vec2i& coordinates_) const;
    };

    using uint = unsigned int;
//    using HeuristicFunction = std::function<uint(Vec2i, Vec2i)>;
    using HeuristicFunction = uint (*)(const Vec2i&, const Vec2i&);
    using RelaxFuncion = float (*)(float e,int n,int len);
//    using CollisionFunction = bool (*)(const Vec2i&);
    using CollisionFunction = std::function<bool(const Vec2i&)>;
    using CoordinateList = std::vector<Vec2i>;

    struct Node
    {
        uint G=0, H=0;
        Vec2i coordinates;
        Node *parent=nullptr;

        Node(const Vec2i &coord_, Node *parent_ = nullptr);
        Node()=default;
        uint getScore() const;
    };

    struct CoordHash
    {
        size_t operator()(const Vec2i &coord) const
        {
            return coord.x*100000+coord.y;
        }
    };

    using NodeHeap=std::vector<Node>;
    using CoordMap = ska::unordered_map<Vec2i,Node,CoordHash>;
    class Generator
    {
        bool detectCollision(const Vec2i &coordinates_);
        Node* findNodeOnMap(CoordMap& nodes_,const Vec2i &coordinates_);
    public:
        Generator();
        void setWorldSize(const Vec2i &worldSize_);
        void setDiagonalMovement(bool enable_);
        void setWeight(float epsilon_);
        void setRelaxFunction(RelaxFuncion relaxer_);
        void setHeuristic(HeuristicFunction heuristic_);
        template<typename F>
        void SetCollision(F &&collison_)
        {
            collision=collison_;
        }
        CoordinateList findPath(const Vec2i &source_,const Vec2i &target_);
        void addCollision(const Vec2i &coordinates_);
        void removeCollision(const Vec2i &coordinates_);
        void clearCollisions();

    private:
        HeuristicFunction heuristic;
        CollisionFunction collision;
        RelaxFuncion      relaxer;
        CoordinateList direction, walls;
        Vec2i worldSize;
        float epsilon;
        uint directions;
    };
    class Relaxer
    {
    public:
        static float dynamic_weight(float e ,int n, int len);
        static float static_weight(float e ,int n, int len);
    };
    class Heuristic
    {
    public:
        static Vec2i getDelta(const Vec2i &source_,const Vec2i &target_);
        static uint manhattan(const Vec2i &source_,const Vec2i &target_);
        static uint euclidean(const Vec2i &source_,const Vec2i &target_);
        static uint octagonal(const Vec2i &source_,const Vec2i &target_);
    };
}

#endif // __ASTAR_HPP__
