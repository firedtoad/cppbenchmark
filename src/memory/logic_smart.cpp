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

#include "utils/logic.h"
#include <deque>
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

    LogicMemoryLinear<std::list<Small>, SIZE, false>();
    LogicMemoryLinear<std::list<std::shared_ptr<Small>>, SIZE, false>();
    LogicMemoryLinear<std::list<std::unique_ptr<Small>>, SIZE, false>();
    LogicMemoryLinear<std::list<Big>, SIZE, false>();
    LogicMemoryLinear<std::list<std::shared_ptr<Big>>, SIZE, false>();
    LogicMemoryLinear<std::list<std::unique_ptr<Big>>, SIZE, false>();

    LogicMemoryLinear<std::deque<Small>, SIZE, false>();
    LogicMemoryLinear<std::deque<std::shared_ptr<Small>>, SIZE, false>();
    LogicMemoryLinear<std::deque<std::unique_ptr<Small>>, SIZE, false>();
    LogicMemoryLinear<std::deque<Big>, SIZE, false>();
    LogicMemoryLinear<std::deque<std::shared_ptr<Big>>, SIZE, false>();
    LogicMemoryLinear<std::deque<std::unique_ptr<Big>>, SIZE, false>();

    LogicMemoryLinear<std::vector<Small>, SIZE>();
    LogicMemoryLinear<std::vector<std::shared_ptr<Small>>, SIZE>();
    LogicMemoryLinear<std::vector<std::unique_ptr<Small>>, SIZE>();
    LogicMemoryLinear<std::vector<Big>, SIZE>();
    LogicMemoryLinear<std::vector<std::shared_ptr<Big>>, SIZE>();
    LogicMemoryLinear<std::vector<std::unique_ptr<Big>>, SIZE>();

    LogicMemoryMapInt<std::map<int, Small>, SIZE, false>();
    LogicMemoryMapInt<std::map<int, std::shared_ptr<Small>>, SIZE, false>();
    LogicMemoryMapInt<std::map<int, std::unique_ptr<Small>>, SIZE, false>();

    LogicMemoryMapInt<std::map<int, Big>, SIZE, false>();
    LogicMemoryMapInt<std::map<int, std::shared_ptr<Big>>, SIZE, false>();
    LogicMemoryMapInt<std::map<int, std::unique_ptr<Big>>, SIZE, false>();

    LogicMemoryMapInt<std::unordered_map<int, Small>, SIZE, true>();
    LogicMemoryMapInt<std::unordered_map<int, std::shared_ptr<Small>>, SIZE, true>();
    LogicMemoryMapInt<std::unordered_map<int, std::unique_ptr<Small>>, SIZE, true>();

    LogicMemoryMapInt<std::unordered_map<int, Big>, SIZE, true>();
    LogicMemoryMapInt<std::unordered_map<int, std::shared_ptr<Big>>, SIZE, true>();
    LogicMemoryMapInt<std::unordered_map<int, std::unique_ptr<Big>>, SIZE, true>();

    LogicMemoryMapString<std::map<std::string, Small>, SIZE, false>();
    LogicMemoryMapString<std::map<std::string, std::shared_ptr<Small>>, SIZE, false>();
    LogicMemoryMapString<std::map<std::string, std::unique_ptr<Small>>, SIZE, false>();

    LogicMemoryMapString<std::map<std::string, Big>, SIZE, false>();
    LogicMemoryMapString<std::map<std::string, std::shared_ptr<Big>>, SIZE, false>();
    LogicMemoryMapString<std::map<std::string, std::unique_ptr<Big>>, SIZE, false>();

    LogicMemoryMapString<std::unordered_map<std::string, Small>, SIZE, true>();
    LogicMemoryMapString<std::unordered_map<std::string, std::shared_ptr<Small>>, SIZE, true>();
    LogicMemoryMapString<std::unordered_map<std::string, std::unique_ptr<Small>>, SIZE, true>();

    LogicMemoryMapString<std::unordered_map<std::string, Big>, SIZE, true>();
    LogicMemoryMapString<std::unordered_map<std::string, std::shared_ptr<Big>>, SIZE, true>();
    LogicMemoryMapString<std::unordered_map<std::string, std::unique_ptr<Big>>, SIZE, true>();

    LogicMemorySet<std::set<Small>, SIZE, false>();
    LogicMemorySet<std::set<std::shared_ptr<Small>>, SIZE, false>();
    LogicMemorySet<std::set<std::unique_ptr<Small>>, SIZE, false>();
    LogicMemorySet<std::set<Big>, SIZE, false>();
    LogicMemorySet<std::set<std::shared_ptr<Big>>, SIZE, false>();
    LogicMemorySet<std::set<std::unique_ptr<Big>>, SIZE, false>();

    LogicMemorySet<std::unordered_set<Small>, SIZE, true>();
    LogicMemorySet<std::unordered_set<std::shared_ptr<Small>>, SIZE, true>();
    LogicMemorySet<std::unordered_set<std::unique_ptr<Small>>, SIZE, true>();
    LogicMemorySet<std::unordered_set<Big>, SIZE, true>();
    LogicMemorySet<std::unordered_set<std::shared_ptr<Big>>, SIZE, true>();
    LogicMemorySet<std::unordered_set<std::unique_ptr<Big>>, SIZE, true>();

    LogicMemorySetString<std::set<std::string>, SIZE, false>();
    LogicMemorySetString<std::unordered_set<std::string>, SIZE, true>();
    return 0;
}