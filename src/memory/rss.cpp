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
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sys/resource.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "utils/rss.h"

const int SIZE = 1024 * 1024;
int main(int argc, char *argv[])
{
    rusage usage{};
    FillRSS(usage);
    std::vector<uint8_t> vecu8;
    vecu8.resize(SIZE);
    std::cout << "vector of u8 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::vector<uint64_t> vec;
    vec.resize(SIZE);
    std::cout << "vector of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::deque<uint8_t> dequ8;
    dequ8.resize(SIZE);
    std::cout << "deque of u8 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::deque<uint64_t> dequ64;
    dequ64.resize(SIZE);
    std::cout << "deque of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::list<uint8_t> li;
    li.resize(SIZE);
    std::cout << "list of u8 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::list<uint16_t> li16;
    li16.resize(SIZE);
    std::cout << "list of u16 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::list<uint64_t> li64;
    li64.resize(SIZE);
    std::cout << "list of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::set<uint32_t> s32;
    for (auto i = 0; i < SIZE; i++)
    {
        s32.insert(i);
    }
    std::cout << "set of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::set<uint64_t> s64;
    for (auto i = 0; i < SIZE; i++)
    {
        s64.insert(i);
    }
    std::cout << "set of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::map<uint32_t, uint32_t> m32;
    for (auto i = 0; i < SIZE; i++)
    {
        m32[i] = i;
    }
    std::cout << "map of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::map<uint64_t, uint64_t> m64;
    for (auto i = 0; i < SIZE; i++)
    {
        m64[i] = i;
    }
    std::cout << "map of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::unordered_set<uint32_t> us32;
    us32.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        us32.insert(i);
    }
    std::cout << "uset of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::unordered_set<uint64_t> us64;
    us64.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        us64.insert(i);
    }
    std::cout << "uset of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::unordered_map<uint32_t, uint32_t> um32;
    um32.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        um32[i] = i;
    }
    std::cout << "umap of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    std::unordered_map<uint64_t, uint64_t> um64;
    um64.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        um64[i] = i;
    }
    std::cout << "umap of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    ska::flat_hash_set<uint32_t> flat_set_32;
    flat_set_32.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        flat_set_32.insert(i);
    }
    std::cout << "ska flat set of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    ska::flat_hash_set<uint64_t> flat_set_64;
    flat_set_64.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        flat_set_64.insert(i);
    }
    std::cout << "ska flat set of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    phmap::flat_hash_set<uint32_t> ph_flat_set_32;
    ph_flat_set_32.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        ph_flat_set_32.insert(i);
    }
    std::cout << "ph flat set of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    phmap::flat_hash_set<uint64_t> ph_flat_set_64;
    ph_flat_set_64.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        ph_flat_set_64.insert(i);
    }
    std::cout << "ph flat set of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    ska::flat_hash_map<uint32_t, uint32_t> flat_map_32;
    flat_map_32.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        flat_map_32[i] = i;
    }
    std::cout << "ska flat map of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    ska::flat_hash_map<uint64_t, uint64_t> flat_map_64;
    flat_map_64.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        flat_map_64[i] = i;
    }
    std::cout << "ska flat map of u64 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    phmap::flat_hash_map<uint32_t, uint32_t> ph_flat_map_32;
    ph_flat_map_32.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        ph_flat_map_32[i] = i;
    }
    std::cout << "ph flat map of u32 " << SIZE << '\n';
    PrintUsage(usage);

    FillRSS(usage);
    phmap::flat_hash_map<uint64_t, uint64_t> ph_flat_map_64;
    ph_flat_map_64.reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        ph_flat_map_64[i] = i;
    }
    std::cout << "ph flat map of u64 " << SIZE << '\n';
    PrintUsage(usage);

    return 0;
}
