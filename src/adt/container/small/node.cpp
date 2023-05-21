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

#include <deque>
#include <iostream>
#include <list>
#include <llvm/ADT/simple_ilist.h>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "utils/symbol.h"


template <typename... T> void PrintVector(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(typename T::value_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintNode(T &&...t)
{
    (..., (std::cout << demangle(typeid(typename T::node_type).name()) << " size " << sizeof(typename T::node_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintContainer(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(typename T::value_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintList(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(t) << '\n'));
    std::cout << '\n';
}

struct LNode : public llvm::ilist_node<LNode, llvm::ilist_tag<LNode>>
{
    int x{};
};

int main(int argc, char *argv[])
{
    PrintVector(std::vector<uint8_t>{}, std::vector<uint16_t>{}, std::vector<uint32_t>{}, std::vector<uint64_t>{});
    PrintVector(std::deque<uint8_t>{}, std::deque<uint16_t>{}, std::deque<uint32_t>{}, std::deque<uint64_t>{});
    PrintVector(std::queue<uint8_t>{}, std::queue<uint16_t>{}, std::queue<uint32_t>{}, std::queue<uint64_t>{});
    PrintNode(std::set<uint8_t>{}, std::set<uint16_t>{}, std::set<uint32_t>{}, std::set<uint64_t>{});
    PrintList(std::_Rb_tree_node<uint8_t>{}, std::_Rb_tree_node<uint16_t>{}, std::_Rb_tree_node<uint32_t>{}, std::_Rb_tree_node<uint64_t>{});
    PrintNode(std::map<uint8_t, uint8_t>{}, std::map<uint16_t, uint16_t>{}, std::map<uint32_t, uint32_t>{}, std::map<uint64_t, uint64_t>{});
    PrintList(std::_Rb_tree_node<std::pair<uint8_t, uint8_t>>{}, std::_Rb_tree_node<std::pair<uint16_t, uint16_t>>{},
              std::_Rb_tree_node<std::pair<uint32_t, uint32_t>>{}, std::_Rb_tree_node<std::pair<uint64_t, uint64_t>>{});
    PrintNode(std::unordered_set<uint8_t>{}, std::unordered_set<uint16_t>{}, std::unordered_set<uint32_t>{}, std::unordered_set<uint64_t>{},
              std::unordered_set<uint64_t>{});
    PrintList(std::__detail::_Hash_node<uint8_t, false>{}, std::__detail::_Hash_node<uint16_t, false>{}, std::__detail::_Hash_node<uint32_t, false>{},
              std::__detail::_Hash_node<uint64_t, false>{});

    PrintNode(std::unordered_map<uint8_t, uint8_t>{}, std::unordered_map<uint16_t, uint16_t>{}, std::unordered_map<uint32_t, uint32_t>{},
              std::unordered_map<uint64_t, uint64_t>{});
    PrintList(std::__detail::_Hash_node<std::pair<uint8_t, uint8_t>, false>{}, std::__detail::_Hash_node<std::pair<uint16_t, uint16_t>, false>{},
              std::__detail::_Hash_node<std::pair<uint32_t, uint32_t>, false>{}, std::__detail::_Hash_node<std::pair<uint64_t, uint64_t>, false>{});

    return 0;
}
