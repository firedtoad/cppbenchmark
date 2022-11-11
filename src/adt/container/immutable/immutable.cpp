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

#include <benchmark/benchmark.h>
#include <iostream>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/ImmutableList.h>
#include <llvm/ADT/ImmutableMap.h>
#include <llvm/ADT/ImmutableSet.h>

struct Node
{
    int i;
    void Profile(llvm::FoldingSetNodeID &ID) const {}
};

int main(int argc, char **argv)
{
    {
        llvm::ImmutableMap<int, int>::Factory factory;
        auto imap = factory.getEmptyMap();
        for (auto i = 0; i < 1024; i++)
        {
            imap = factory.add(imap, i, i);
        }
        for (auto &it : imap)
        {
            std::cout << it.first << ":" << it.second << '\n';
        }
    }
    {
        llvm::ImmutableSet<int>::Factory factory;

        auto imset = factory.getEmptySet();
        for (auto i = 0; i < 1024; i++)
        {
            imset = factory.add(imset, i);
        }
        for (auto &it : imset)
        {
            std::cout << it << '\n';
        }
    }
    {
        llvm::ImmutableList<Node>::Factory factory;
        auto list = factory.getEmptyList();
        for (auto i = 0; i < 10; i++)
        {
            list = factory.emplace(list, Node{i});
        }
        for (auto &it : list)
        {
            std::cout << it.i << '\n';
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}