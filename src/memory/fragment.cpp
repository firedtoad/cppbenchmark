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


#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "utils/rss.h"
#include "utils/logic.h"


void MemoryAlloc(std::vector<int> &sz)
{
    std::vector<void *> vec(sz.size());
    AllocReport report{"MemoryAlloc"};
    boost::pool p(128);
    for (size_t i = 0; i < vec.size(); i++)
    {
        vec[i] = malloc(sz[i]);
    }
    for (auto it : vec)
    {
        free(it);
    }
}


struct BigData
{
    char c[40];
};

static const int SIZE = 1 << 30;
int main(int argc, char **argv)
{
//    {
//        std::cout << "allocate 16-4096 bytes" << '\n';
//        std::vector<int> vecSize;
//        for (auto sz = 0; sz < SIZE;)
//        {
//            auto n = 16 + std::rand() % 4080;
//            vecSize.emplace_back(n);
//            sz += n;
//        }
//        MemoryAlloc(vecSize);
//    }
//    {
//        std::cout << "allocate 8-4024 bytes" << '\n';
//        std::vector<int> vecSize;
//        for (auto sz = 0; sz < SIZE;)
//        {
//            auto n = 8 + std::rand() % 1016;
//            vecSize.emplace_back(n);
//            sz += n;
//        }
//        MemoryAlloc(vecSize);
//    }
//    {
//        std::cout << "allocate 8-512 bytes" << '\n';
//        std::vector<int> vecSize;
//        for (auto sz = 0; sz < SIZE;)
//        {
//            auto n = 8 + std::rand() % 504;
//            vecSize.emplace_back(n);
//            sz += n;
//        }
//        MemoryAlloc(vecSize);
//    }
//    {
//        std::cout << "allocate 8-128 bytes" << '\n';
//        std::vector<int> vecSize;
//        for (auto sz = 0; sz < SIZE;)
//        {
//            auto n = 8 + std::rand() % 120;
//            vecSize.emplace_back(n);
//            sz += n;
//        }
//        MemoryAlloc(vecSize);
//    }
//    {
//        std::cout << "allocate 8-64 bytes" << '\n';
//        std::vector<int> vecSize;
//        for (auto sz = 0; sz < SIZE;)
//        {
//            auto n = 8 + std::rand() % 56;
//            vecSize.emplace_back(n);
//            sz += n;
//        }
//        MemoryAlloc(vecSize);
//    }
//    {
//        std::cout << "allocate 8-32 bytes" << '\n';
//        std::vector<int> vecSize;
//        for (auto sz = 0; sz < SIZE;)
//        {
//            auto n = 8 + std::rand() % 24;
//            vecSize.emplace_back(n);
//            sz += n;
//        }
//        MemoryAlloc(vecSize);
//    }
    rusage rUsage;
    std::vector<std::shared_ptr<BigData>> vec(1<<20);
//    MemoryPool<BigData> pool;

//    FillRSS(rUsage);
    AllocReport report{"MemoryAlloc"};
    std::make_shared<int>();
    for(auto i=0;i<vec.size();i++)
    {
//        vec[i]= std::allocate_shared<int,boost::fast_pool_allocator<int>>(alloc,i);
//        vec[i].reset(pool.newElement(),[&pool](BigData* p){pool.deleteElement(p);});
//        vec[i]=std::make_shared<BigData>();
//        vec[i].reset(new uint64_t);
    }
//    PrintUsage(rUsage);
    return 0;
}