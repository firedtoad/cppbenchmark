#include "AStar.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
bool AStar::Vec2i::operator==(const Vec2i &coordinates_) const
{
    return (x == coordinates_.x && y == coordinates_.y);
}
std::ostream &operator<<(std::ostream &os, const AStar::Vec2i &coord_)
{
    return os << coord_.x << " " << coord_.y;
}

AStar::Vec2i operator+(const AStar::Vec2i &left_, const AStar::Vec2i &right_)
{
    return {left_.x + right_.x, left_.y + right_.y};
}

AStar::Node::Node(const Vec2i &coordinates_, Node *parent_)
{
    parent      = parent_;
    coordinates = coordinates_;
    G = H = 0;
}

AStar::uint AStar::Node::getScore() const
{
    return G + H;
}

AStar::Generator::Generator() : collision(nullptr)
{
    setDiagonalMovement(false);
    setHeuristic(&Heuristic::manhattan);
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

void AStar::Generator::addCollision(const Vec2i &coordinates_)
{
    walls.push_back(coordinates_);
}

void AStar::Generator::removeCollision(const Vec2i &coordinates_)
{
    auto it = std::find(walls.begin(), walls.end(), coordinates_);
    if (it != walls.end())
    {
        walls.erase(it);
    }
}

void AStar::Generator::clearCollisions()
{
    walls.clear();
}

AStar::CoordinateList AStar::Generator::findPath(const Vec2i &source_, const Vec2i &target_)
{
    if (detectCollision(source_) || detectCollision(target_))
    {
        return {};
    }
    auto delta    = Heuristic::getDelta(source_, target_);
    auto dist     = std::max(delta.x, delta.y) + 1;
    Node *current = nullptr;
    CordSet openSet;
    std::vector<Node> openHeap;
    openHeap.reserve(dist * 5);
    CoordMap closedMap;
    openSet.reserve(dist * 6);
    closedMap.reserve(dist * 2);
    openHeap.emplace_back(source_);
    std::push_heap(openHeap.begin(), openHeap.end(), comp);
    openSet.emplace(source_);
    while (!openHeap.empty())
    {
        current = &openHeap.front();
        if (current->coordinates == target_)
        {
            break;
        }
        auto coord       = current->coordinates;
        closedMap[coord] = current;
        std::pop_heap(openHeap.begin(), openHeap.end(), comp);
        openHeap.pop_back();
        openSet.erase(coord);
        current = closedMap[coord];
        for (uint i = 0; i < directions; ++i)
        {
            Vec2i newCoordinates(current->coordinates + direction[i]);
            if (openSet.find(newCoordinates) != openSet.end())
            {
                continue;
            }
            Node *successor = nullptr;
            if (detectCollision(newCoordinates) || (successor = findNodeOnMap(closedMap, newCoordinates)))
            {
                continue;
            }
            uint totalCost = current->G + ((i < 4) ? 10 : 14);
            if (successor == nullptr)
            {
                openHeap.emplace_back(newCoordinates, current);
                auto &ref = openHeap.back();
                ref.G     = totalCost;
                ref.H     = heuristic(ref.coordinates, target_);
                std::push_heap(openHeap.begin(), openHeap.end(), comp);
                openSet.emplace(std::move(newCoordinates));
            }
            else if (totalCost < successor->G)
            {
                successor->parent = current;
                successor->G      = totalCost;
            }
        }
    }

    CoordinateList path;
    bool reverse = false;
    if (current->coordinates == target_)
    {
        reverse = true;
    }
    while (current != nullptr)
    {
        if (reverse)
        {
            path.emplace(path.begin(), std::move(current->coordinates));
        }
        else
        {
            path.emplace_back(std::move(current->coordinates));
        }
        current = current->parent;
    }
    return path;
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
    if (coordinates_.x < 0 || coordinates_.x >= worldSize.x || coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
        std::find(walls.begin(), walls.end(), coordinates_) != walls.end() || (collision && collision(coordinates_)))
    {
        return true;
    }
    return false;
}

AStar::Vec2i AStar::Heuristic::getDelta(const Vec2i &source_, const Vec2i &target_)
{
    return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
}

AStar::uint AStar::Heuristic::manhattan(const Vec2i &source_, const Vec2i &target_)
{
    const auto &delta = getDelta(source_, target_);
    //    if(delta.x>delta.y)
    //    {
    //        return static_cast<uint>(10*delta.x + 4*delta.y);
    //    }else{
    //        return static_cast<uint>(4*delta.x + 10*delta.y);
    //    }
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
