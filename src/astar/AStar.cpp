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
#include "memorypool.h"
#include <algorithm>
#include <cmath>
#include <ostream>
#include <random>
#include <stack>

bool AStar::Vec2i::operator==(const Vec2i &coordinates_) const
{
    return (x == coordinates_.x && y == coordinates_.y);
}

inline std::ostream &operator<<(std::ostream &os, const AStar::Vec2i &coord_)
{
    return os << coord_.x << " " << coord_.y;
}

void operator+=(AStar::Vec2i &left_, const AStar::Vec2i &right_)
{
    left_.x += right_.x;
    left_.y += right_.y;
}

void operator-=(AStar::Vec2i &left_, const AStar::Vec2i &right_)
{
    left_.x -= right_.x;
    left_.y -= right_.y;
}

AStar::Node::Node(const Vec2i &coordinates_, Node *parent_)
{
    parent      = parent_;
    coordinates = coordinates_;
    G = H = 0;
}

AStar::uint AStar::Node::getScore() const
{
    return S;
}

void AStar::Node::updateScore()
{
    S = G + H;
}

AStar::Generator::Generator() : collision(nullptr)
{
    setDiagonalMovement(true);
    setHeuristic(&Heuristic::octagonal);
    setRelaxFunction(&Relaxer::static_weight);
    setWeight(2.0f);
    direction = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
}

void AStar::Generator::setWorldSize(const Vec2i &worldSize_)
{
    worldSize = worldSize_;
}

void AStar::Generator::setDiagonalMovement(bool enable_)
{
    directions = (enable_ ? 8 : 4);
}

void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
    heuristic = heuristic_;
}

auto comp = [](const AStar::Node *pNode1, const AStar::Node *pNode2) { return pNode1->getScore() > pNode2->getScore(); };

AStar::CoordinateList AStar::Generator::findPath(const Vec2i &source_, const Vec2i &target_)
{
    if (detectCollision(source_) || detectCollision(target_))
    {
        return {};
    }
    MemoryPool<AStar::Node> pool;
    auto delta    = Heuristic::getDelta(source_, target_);
    auto dist     = std::max(delta.x, delta.y) + 1;
    Node *current = nullptr;
    CordSet openSet;
    NodeHeap openHeap;
    openHeap.reserve(dist * 4);
    CoordMap closedMap;
    openSet.reserve(dist * 4);
    closedMap.reserve(dist * 4);
    openHeap.emplace_back(pool.newElement(target_));
    std::push_heap(openHeap.begin(), openHeap.end(), comp);
    openSet.emplace(target_);
    int n             = 0;
    int N             = worldSize.x * worldSize.y;
    bool reach_target = false;
    while (!openHeap.empty())
    {
        current = openHeap.front();
        if (current->coordinates == source_)
        {
            reach_target = true;
            break;
        }
        auto coord       = current->coordinates;
        closedMap[coord] = current;
        std::pop_heap(openHeap.begin(), openHeap.end(), comp);
        openHeap.pop_back();
        openSet.erase(coord);
        current      = closedMap[coord];
        float weight = relaxer(epsilon, ++n, N);
        Vec2i newCoordinates{};
        for (uint i = 0; i < directions; ++i)
        {
            newCoordinates = current->coordinates;
            for (auto s = 0; s < step; s++)
            {
                newCoordinates += direction[i];
                if (detectCollision(newCoordinates))
                {
                    newCoordinates -= direction[i];
                    break;
                }
            }
            if (newCoordinates == current->coordinates)
            {
                continue;
            }
            if (openSet.find(newCoordinates) != openSet.end())
            {
                continue;
            }
            Node *successor = nullptr;

            successor      = findNodeOnMap(closedMap, newCoordinates);
            uint totalCost = current->G + ((i < 4) ? 10 : 14);
            if (successor == nullptr)
            {
                openHeap.emplace_back(pool.newElement(newCoordinates, current));
                auto &ref = openHeap.back();
                ref->G    = totalCost;
                ref->H    = weight * heuristic(ref->coordinates, source_);
                ref->updateScore();
                std::push_heap(openHeap.begin(), openHeap.end(), comp);
                openSet.emplace(newCoordinates);
            }
            else if (totalCost < successor->G)
            {
                successor->parent = current;
                successor->G      = totalCost;
                successor->updateScore();
            }
        }
    }

    CoordinateList path;
    while (current != nullptr && reach_target)
    {
        path.emplace_back(current->coordinates);
        current = current->parent;
    }
    return path;
}

AStar::CoordinateList AStar::Generator::findPathStack(const Vec2i &source_, const Vec2i &target_)
{
    if (detectCollision(source_) || detectCollision(target_))
    {
        return {};
    }

    auto delta = Heuristic::getDelta(source_, target_);
    auto dist  = std::max(delta.x, delta.y) + 1;

    CordSet closedSet;
    closedSet.reserve(dist * 4);
    std::vector<Vec2i> openStack;
    openStack.reserve(dist);
    openStack.push_back(source_);
    Vec2i newPoint{};
    int n             = 0;
    int N             = worldSize.x * worldSize.y;
    auto dirs         = direction;
    bool reach_target = false;
    while (!openStack.empty())
    {
        auto point = openStack.back();
        delta      = Heuristic::getDelta(point, target_);
        dist       = std::max(delta.x, delta.y);
        if (point == target_ || dist <= step)
        {
            reach_target = true;
            break;
        }
        closedSet.emplace(point);
        // 是否发现新节点
        bool bFound    = false;
        auto min_score = UINT_MAX;
        n++;
        float wh = relaxer(epsilon, ++n, N);
        for (uint i = 0; i < directions; ++i)
        {
            auto nPoint = point;
            for (auto s = 0; s < step; s++)
            {
                nPoint += direction[i];
                if (detectCollision(nPoint))
                {
                    nPoint -= direction[i];
                    break;
                }
            }
            if (nPoint == point)
            {
                continue;
            }
            // 已经探测过跳过
            if (closedSet.find(nPoint) != closedSet.end())
            {
                continue;
            }
            auto score = (uint)(wh * heuristic(nPoint, target_));
            if (score < min_score)
            {
                min_score = score;
                newPoint  = nPoint;
                bFound    = true;
            }
        }
        if (bFound)
        {
            openStack.push_back(newPoint);
        }
        else
        {
            openStack.pop_back();
        }
    }
    if (reach_target)
    {
        return openStack;
    }
    return {};
}

AStar::Node *AStar::Generator::findNodeOnMap(CoordMap &nodes_, const Vec2i &coordinates_)
{
    const auto &it = nodes_.find(coordinates_);
    if (it != nodes_.end())
    {
        return it->second;
    }
    return nullptr;
}

bool AStar::Generator::detectCollision(const Vec2i &coordinates_)
{
    int x = coordinates_.x;
    int y = coordinates_.y;
    if (x < 0 || x >= worldSize.x || y < 0 || y >= worldSize.y || (collision && collision(coordinates_)))
    {
        return true;
    }
    return false;
}

void AStar::Generator::setRelaxFunction(AStar::RelaxFunction relaxer_)
{
    relaxer = relaxer_;
}

void AStar::Generator::setWeight(float epsilon_)
{
    epsilon = epsilon_;
}

void AStar::Generator::setStep(int32_t step_)
{
    step = step_;
}

float AStar::Relaxer::static_weight(float e, int n, int len)
{
    return e;
}

float AStar::Relaxer::dynamic_weight(float e, int n, int len)
{
    float w = 0;
    if (n <= len)
    {
        w = 1.0f - n / (len + .0f);
    }
    return 1.0f + e * w;
}

AStar::Vec2i AStar::Heuristic::getDelta(const Vec2i &source_, const Vec2i &target_)
{
    return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
}

AStar::uint AStar::Heuristic::manhattan(const Vec2i &source_, const Vec2i &target_)
{
    const auto &delta = getDelta(source_, target_);
    return static_cast<uint>(10 * (delta.x + delta.y));
}

AStar::uint AStar::Heuristic::euclidean(const Vec2i &source_, const Vec2i &target_)
{
    const auto &delta = getDelta(source_, target_);
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

AStar::uint AStar::Heuristic::octagonal(const Vec2i &source_, const Vec2i &target_)
{
    const auto &delta = getDelta(source_, target_);
    return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}
