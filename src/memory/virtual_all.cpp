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

#include "utils/rss.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <sys/mman.h>
#include <unistd.h>

const int PAGE_SIZE = sysconf(_SC_PAGESIZE);

void PrintMMap()
{
    std::fstream fs("/proc/self/maps", std::ios::binary | std::ios::in);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::cout << ss.str() << '\n';
}

struct Address
{
    uint64_t start;
    uint64_t end;
    uint64_t size;
};

std::vector<Address> GetAddrMap()
{
    std::vector<Address> ret;
    std::fstream fs("/proc/self/maps", std::ios::binary | std::ios::in);
    std::string line;
    while (std::getline(fs, line))
    {
        uint64_t start{};
        uint64_t end{};
        sscanf(line.c_str(), "%lx-%lx", &start, &end);
        ret.emplace_back(Address{start, end, end - start});
    }
    std::sort(ret.begin(), ret.end(), [](auto p1, auto p2) { return p1.start < p2.start; });
    return ret;
}

// https://www.sandpile.org/x86/can_addr.htm
// echo 0 > /proc/sys/vm/mmap_min_addr
// vm.overcommit_memory = 1
// vm.mmap_min_addr = 0
int main(int argc, char **argv)
{
    const auto addrEnd = 0x7FFFFFFFFFFF;
    std::vector<Address> vecFree;
    vecFree.reserve(1024);
    auto addrMap   = GetAddrMap();
    uint64_t start = 0;
    for (auto &&it : addrMap)
    {
        if (it.start != start)
        {
            vecFree.emplace_back(Address{start, it.start, it.start - start});
        }
        start = it.end;
    }

    vecFree.emplace_back(Address{start, addrEnd, (addrEnd - start) / 4096 * 4096});
    uint64_t nAllSize=0;
    PrintMMap();
//    for (auto &&it : vecFree)
    ////    {
    ////        std::cout << std::hex << it.start << "<---->" << it.end << " " << it.size << '\n';
    ////    }
    ////
    ////    for (auto &&it : vecFree)
    ////    {
    ////        mmap(reinterpret_cast<void *>(it.start), it.size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    ////        std::cout << errno << strerror(errno) << '\n';
    ////        nAllSize+=it.size;
    ////    }

    double value = 0.12342;
    value = std::round(value * 100.0) / 100.0;
    std::cout << value << std::endl; // prints 0.13

    std::cout << nAllSize << '\n';
    std::cin.get();

    return 0;
}