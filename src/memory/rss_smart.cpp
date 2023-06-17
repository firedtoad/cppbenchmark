// Copyright 2023 The Division Authors.
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

#include "utils/memory.h"
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Small
{
    uint64_t m_data{};
    Small() noexcept = default;
    Small(uint64_t i) noexcept
    {
        m_data = i;
    }
    bool operator<(const Small &o) const
    {
        return m_data < o.m_data;
    }
    bool operator==(const Small &o) const
    {
        return m_data == o.m_data;
    }
};

struct Big
{
    uint64_t m_data[8]{};
    Big() noexcept = default;
    template <typename T> Big(T &&data)
    {
        m_data[0] = data;
    }
    bool operator<(const Big &o) const
    {
        return m_data[0] < o.m_data[0];
    }
    bool operator==(const Big &o) const
    {
        return m_data == o.m_data;
    }
};

namespace std
{
template <> struct hash<Big>
{
    size_t operator()(const Big &o) const
    {
        return o.m_data[0];
    }
};
template <> struct hash<Small>
{
    size_t operator()(const Small &o) const
    {
        return o.m_data;
    }
};
} // namespace std

constexpr static int SIZE = 1 << 20;

int main(int argc, char *argv[])
{

    RSSMemoryLinear<std::list<Small>, SIZE, false>();
    RSSMemoryLinear<std::list<std::shared_ptr<Small>>, SIZE, false>();
    RSSMemoryLinear<std::list<std::unique_ptr<Small>>, SIZE, false>();
    RSSMemoryLinear<std::list<Big>, SIZE, false>();
    RSSMemoryLinear<std::list<std::shared_ptr<Big>>, SIZE, false>();
    RSSMemoryLinear<std::list<std::unique_ptr<Big>>, SIZE, false>();

    RSSMemoryForward<std::forward_list<Small>, SIZE, false>();
    RSSMemoryForward<std::forward_list<std::shared_ptr<Small>>, SIZE, false>();
    RSSMemoryForward<std::forward_list<std::unique_ptr<Small>>, SIZE, false>();
    RSSMemoryForward<std::forward_list<Big>, SIZE, false>();
    RSSMemoryForward<std::forward_list<std::shared_ptr<Big>>, SIZE, false>();
    RSSMemoryForward<std::forward_list<std::unique_ptr<Big>>, SIZE, false>();

    RSSMemoryLinear<std::deque<Small>, SIZE, false>();
    RSSMemoryLinear<std::deque<std::shared_ptr<Small>>, SIZE, false>();
    RSSMemoryLinear<std::deque<std::unique_ptr<Small>>, SIZE, false>();
    RSSMemoryLinear<std::deque<Big>, SIZE, false>();
    RSSMemoryLinear<std::deque<std::shared_ptr<Big>>, SIZE, false>();
    RSSMemoryLinear<std::deque<std::unique_ptr<Big>>, SIZE, false>();

    RSSMemoryLinear<std::vector<Small>, SIZE, true>();
    RSSMemoryLinear<std::vector<std::shared_ptr<Small>>, SIZE, true>();
    RSSMemoryLinear<std::vector<std::unique_ptr<Small>>, SIZE, true>();
    RSSMemoryLinear<std::vector<Big>, SIZE, true>();
    RSSMemoryLinear<std::vector<std::shared_ptr<Big>>, SIZE, true>();
    RSSMemoryLinear<std::vector<std::unique_ptr<Big>>, SIZE, true>();

    RSSMemoryMapInt<std::map<int, Small>, SIZE, false>();
    RSSMemoryMapInt<std::map<int, std::shared_ptr<Small>>, SIZE, false>();
    RSSMemoryMapInt<std::map<int, std::unique_ptr<Small>>, SIZE, false>();

    RSSMemoryMapInt<std::map<int, Big>, SIZE, false>();
    RSSMemoryMapInt<std::map<int, std::shared_ptr<Big>>, SIZE, false>();
    RSSMemoryMapInt<std::map<int, std::unique_ptr<Big>>, SIZE, false>();

    RSSMemoryMapInt<std::unordered_map<int, Small>, SIZE, true>();
    RSSMemoryMapInt<std::unordered_map<int, std::shared_ptr<Small>>, SIZE, true>();
    RSSMemoryMapInt<std::unordered_map<int, std::unique_ptr<Small>>, SIZE, true>();

    RSSMemoryMapInt<std::unordered_map<int, Big>, SIZE, true>();
    RSSMemoryMapInt<std::unordered_map<int, std::shared_ptr<Big>>, SIZE, true>();
    RSSMemoryMapInt<std::unordered_map<int, std::unique_ptr<Big>>, SIZE, true>();

    RSSMemoryMapString<std::map<std::string, Small>, SIZE, false>();
    RSSMemoryMapString<std::map<std::string, std::shared_ptr<Small>>, SIZE, false>();
    RSSMemoryMapString<std::map<std::string, std::unique_ptr<Small>>, SIZE, false>();

    RSSMemoryMapString<std::map<std::string, Big>, SIZE, false>();
    RSSMemoryMapString<std::map<std::string, std::shared_ptr<Big>>, SIZE, false>();
    RSSMemoryMapString<std::map<std::string, std::unique_ptr<Big>>, SIZE, false>();

    RSSMemoryMapString<std::unordered_map<std::string, Small>, SIZE, true>();
    RSSMemoryMapString<std::unordered_map<std::string, std::shared_ptr<Small>>, SIZE, true>();
    RSSMemoryMapString<std::unordered_map<std::string, std::unique_ptr<Small>>, SIZE, true>();

    RSSMemoryMapString<std::unordered_map<std::string, Big>, SIZE, true>();
    RSSMemoryMapString<std::unordered_map<std::string, std::shared_ptr<Big>>, SIZE, true>();
    RSSMemoryMapString<std::unordered_map<std::string, std::unique_ptr<Big>>, SIZE, true>();

    RSSMemorySet<std::set<Small>, SIZE, false>();
    RSSMemorySet<std::set<std::shared_ptr<Small>>, SIZE, false>();
    RSSMemorySet<std::set<std::unique_ptr<Small>>, SIZE, false>();
    RSSMemorySet<std::set<Big>, SIZE, false>();
    RSSMemorySet<std::set<std::shared_ptr<Big>>, SIZE, false>();
    RSSMemorySet<std::set<std::unique_ptr<Big>>, SIZE, false>();

    RSSMemorySet<std::unordered_set<Small>, SIZE, true>();
    RSSMemorySet<std::unordered_set<std::shared_ptr<Small>>, SIZE, true>();
    RSSMemorySet<std::unordered_set<std::unique_ptr<Small>>, SIZE, true>();
    RSSMemorySet<std::unordered_set<Big>, SIZE, true>();
    RSSMemorySet<std::unordered_set<std::shared_ptr<Big>>, SIZE, true>();
    RSSMemorySet<std::unordered_set<std::unique_ptr<Big>>, SIZE, true>();

    RSSMemorySetString<std::set<std::string>, SIZE, false>();
    RSSMemorySetString<std::unordered_set<std::string>, SIZE, true>();
    return 0;
}