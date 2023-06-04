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
#include "utils/rss.h"
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
    Small(uint64_t i) noexcept {}
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

    BM_MemoryLinear<std::list<Small>, SIZE>();
    BM_MemoryLinear<std::list<std::shared_ptr<Small>>, SIZE>();
    BM_MemoryLinear<std::list<std::unique_ptr<Small>>, SIZE>();
    BM_MemoryLinear<std::list<Big>, SIZE>();
    BM_MemoryLinear<std::list<std::shared_ptr<Big>>, SIZE>();
    BM_MemoryLinear<std::list<std::unique_ptr<Big>>, SIZE>();

    BM_MemoryLinear<std::deque<Small>, SIZE>();
    BM_MemoryLinear<std::deque<std::shared_ptr<Small>>, SIZE>();
    BM_MemoryLinear<std::deque<std::unique_ptr<Small>>, SIZE>();
    BM_MemoryLinear<std::deque<Big>, SIZE>();
    BM_MemoryLinear<std::deque<std::shared_ptr<Big>>, SIZE>();
    BM_MemoryLinear<std::deque<std::unique_ptr<Big>>, SIZE>();

    BM_MemoryLinear<std::vector<Small>, SIZE, true>();
    BM_MemoryLinear<std::vector<std::shared_ptr<Small>>, SIZE, true>();
    BM_MemoryLinear<std::vector<std::unique_ptr<Small>>, SIZE, true>();
    BM_MemoryLinear<std::vector<Big>, SIZE, true>();
    BM_MemoryLinear<std::vector<std::shared_ptr<Big>>, SIZE, true>();
    BM_MemoryLinear<std::vector<std::unique_ptr<Big>>, SIZE, true>();

    BM_MemoryMapInt<std::map<int, Small>, SIZE>();
    BM_MemoryMapInt<std::map<int, std::shared_ptr<Small>>,SIZE>();
    BM_MemoryMapInt<std::map<int, std::unique_ptr<Small>>,SIZE>();

    BM_MemoryMapInt<std::map<int, Big>,SIZE>();
    BM_MemoryMapInt<std::map<int, std::shared_ptr<Big>>,SIZE>();
    BM_MemoryMapInt<std::map<int, std::unique_ptr<Big>>,SIZE>();

    BM_MemoryMapInt<std::unordered_map<int, Small>,SIZE, true>();
    BM_MemoryMapInt<std::unordered_map<int, std::shared_ptr<Small>>,SIZE, true>();
    BM_MemoryMapInt<std::unordered_map<int, std::unique_ptr<Small>>,SIZE, true>();

    BM_MemoryMapInt<std::unordered_map<int, Big>,SIZE, true>();
    BM_MemoryMapInt<std::unordered_map<int, std::shared_ptr<Big>>,SIZE, true>();
    BM_MemoryMapInt<std::unordered_map<int, std::unique_ptr<Big>>,SIZE, true>();

    BM_MemoryMapString<std::map<std::string, Small>,SIZE>();
    BM_MemoryMapString<std::map<std::string, std::shared_ptr<Small>>,SIZE>();
    BM_MemoryMapString<std::map<std::string, std::unique_ptr<Small>>,SIZE>();

    BM_MemoryMapString<std::map<std::string, Big>,SIZE>();
    BM_MemoryMapString<std::map<std::string, std::shared_ptr<Big>>,SIZE>();
    BM_MemoryMapString<std::map<std::string, std::unique_ptr<Big>>,SIZE>();

    BM_MemoryMapString<std::unordered_map<std::string, Small>, true>();
    BM_MemoryMapString<std::unordered_map<std::string, std::shared_ptr<Small>>, true>();
    BM_MemoryMapString<std::unordered_map<std::string, std::unique_ptr<Small>>, true>();

    BM_MemoryMapString<std::unordered_map<std::string, Big>, true>();
    BM_MemoryMapString<std::unordered_map<std::string, std::shared_ptr<Big>>, true>();
    BM_MemoryMapString<std::unordered_map<std::string, std::unique_ptr<Big>>, true>();

    BM_MemorySetInt<std::set<Small>, SIZE>();
    BM_MemorySetInt<std::set<std::shared_ptr<Small>>, SIZE>();
    BM_MemorySetInt<std::set<std::unique_ptr<Small>>, SIZE>();
    BM_MemorySetInt<std::set<Big>, SIZE>();
    BM_MemorySetInt<std::set<std::shared_ptr<Big>>, SIZE>();
    BM_MemorySetInt<std::set<std::unique_ptr<Big>>, SIZE>();

    BM_MemorySetInt<std::unordered_set<Small>, SIZE, true>();
    BM_MemorySetInt<std::unordered_set<std::shared_ptr<Small>>, SIZE, true>();
    BM_MemorySetInt<std::unordered_set<std::unique_ptr<Small>>, SIZE, true>();
    BM_MemorySetInt<std::unordered_set<Big>, SIZE, true>();
    BM_MemorySetInt<std::unordered_set<std::shared_ptr<Big>>, SIZE, true>();
    BM_MemorySetInt<std::unordered_set<std::unique_ptr<Big>>, SIZE, true>();

    BM_MemorySetString<std::set<std::string>, SIZE>();
    BM_MemorySetString<std::set<std::shared_ptr<std::string>>, SIZE>();
    BM_MemorySetString<std::set<std::unique_ptr<std::string>>, SIZE>();

    BM_MemorySetString<std::unordered_set<std::string>, SIZE, true>();
    BM_MemorySetString<std::unordered_set<std::shared_ptr<std::string>>, SIZE, true>();
    BM_MemorySetString<std::unordered_set<std::unique_ptr<std::string>>, SIZE, true>();
}