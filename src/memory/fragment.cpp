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
#include <iostream>
#include <memory>
#include <vector>

void MemoryAlloc(std::vector<int> &sz)
{
    std::vector<void *> vec(sz.size());
    AllocReport report{"MemoryAlloc"};
    for (size_t i = 0; i < vec.size(); i++)
    {
        vec[i] = malloc(sz[i]);
    }
    for (auto it : vec)
    {
        free(it);
    }
}

static const int SIZE = 1 << 30;
int main(int argc, char **argv)
{
    {
        std::vector<int> vecSize;
        for (auto sz = 0; sz < SIZE;)
        {
            auto n = 16 + std::rand() % 4080;
            vecSize.emplace_back(n);
            sz += n;
        }
        MemoryAlloc(vecSize);
    }
    {
        std::vector<int> vecSize;
        for (auto sz = 0; sz < SIZE;)
        {
            auto n = 8 + std::rand() % 1016;
            vecSize.emplace_back(n);
            sz += n;
        }
        MemoryAlloc(vecSize);
    }
    {
        std::vector<int> vecSize;
        for (auto sz = 0; sz < SIZE;)
        {
            auto n = 8 + std::rand() % 504;
            vecSize.emplace_back(n);
            sz += n;
        }
        MemoryAlloc(vecSize);
    }
    {
        std::vector<int> vecSize;
        for (auto sz = 0; sz < SIZE;)
        {
            auto n = 8 + std::rand() % 120;
            vecSize.emplace_back(n);
            sz += n;
        }
        MemoryAlloc(vecSize);
    }
    {
        std::vector<int> vecSize;
        for (auto sz = 0; sz < SIZE;)
        {
            auto n = 8 + std::rand() % 56;
            vecSize.emplace_back(n);
            sz += n;
        }
        MemoryAlloc(vecSize);
    }
    {
        std::vector<int> vecSize;
        for (auto sz = 0; sz < SIZE;)
        {
            auto n = 8 + std::rand() % 24;
            vecSize.emplace_back(n);
            sz += n;
        }
        MemoryAlloc(vecSize);
    }
    return 0;
}