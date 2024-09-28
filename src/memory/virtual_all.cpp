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

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/mman.h>
#include <unistd.h>
#include <chrono>
#include <string.h>

const int PAGE_SIZE = sysconf(_SC_PAGESIZE);
const size_t DEFAULT_MAP_WINDOW ((1UL << 47) - PAGE_SIZE);
void PrintMMap() {
    std::fstream fs("/proc/self/maps", std::ios::binary | std::ios::in);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::cout << ss.str() << std::endl;
}

struct Address {
    uint64_t start{};
    uint64_t end{};
    uint64_t size{};

    uint8_t perm{};
    size_t offset{};
    char dev[5]{};
    size_t inode{};
    char path[256]{};
};

std::vector<Address> GetAddrMap() {
    std::vector<Address> ret;
    std::fstream fs("/proc/self/maps", std::ios::binary | std::ios::in);
    std::string line;
    while (std::getline(fs, line)) {
        auto &addr = ret.emplace_back();
        char r{}, w{}, e{}, p{};
        sscanf(line.c_str(), "%lx-%lx %c%c%c%c %lx %s %lu %s", &addr.start, &addr.end,
               &r, &w, &e, &p, &addr.offset, addr.dev, &addr.inode, addr.path);
        addr.size = addr.end - addr.start;
    }
    std::sort(ret.begin(), ret.end(), [](auto p1, auto p2) { return p1.start < p2.start; });
    return ret;
}


// https://www.sandpile.org/x86/can_addr.htm
// echo 0 > /proc/sys/vm/mmap_min_addr
// vm.overcommit_memory = 1
// vm.mmap_min_addr = 0

constexpr auto MM_LOWEST_USER_ADDRESS = 0x10000;
constexpr auto MI_HIGHEST_USER_ADDRESS = 0x80000000000 - 0x10000 - 1; // highest user address

int main(int argc, char **argv) {
    const auto addrEnd = DEFAULT_MAP_WINDOW;
    std::vector<Address> vecFree;
    vecFree.reserve(1024);
    auto addrMap = GetAddrMap();
    uint64_t start = 0;
    for (auto &&it: addrMap) {
        if (it.start > addrEnd) {
            break;
        }
        if (it.start != start) {
            vecFree.emplace_back(Address{start, it.start, it.start - start,0});
        }
        start = it.end;
    }

    vecFree.emplace_back(Address{start, addrEnd, (addrEnd - start) / 4096 * 4096,0});
    uint64_t nAllSize = 0;
    for (auto &&it: vecFree) {
        std::cout << std::hex << it.start << "<---->" << it.end << " " << it.size << std::endl;
    }
    std::cout << std::dec << nAllSize / 1024 / 1024 << "MB" << std::endl;
    uint64_t res = 0;
    size_t len = 0;
    for (auto it: addrMap) {
        if(it.start> addrEnd)
        {
            break;
        }
        nAllSize += it.size;
        if (strncmp(it.path,"[vvar]",sizeof("[vvar]"))!=0) {
            mprotect((void *) it.start, it.size, PROT_WRITE | PROT_READ | PROT_EXEC);
        }
        std::cout << std::hex << it.start << "<---->" << it.end << " " << it.size << " error = " << errno << " desc = "
                  << strerror(errno) << std::endl;

    }

    for (auto &&it: vecFree) {
        mmap(reinterpret_cast<void *>(it.start), it.size, PROT_READ | PROT_WRITE | PROT_EXEC,
             MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        std::cout << std::hex << it.start << "<---->" << it.end << " " << it.size << " error=" << errno << "=>"
                  << strerror(errno) << std::endl;
        nAllSize += it.size;
    }

    auto nStart = std::chrono::steady_clock::now();

    PrintMMap();
    for (auto it: addrMap) {
        if(it.start> addrEnd)
        {
            break;
        }
        std::cout << "start travel from " << std::hex << it.start << "<---->" << it.end << " " << it.size << std::endl;
        size_t skip = PAGE_SIZE;
        auto p = it.start;
        for (; p < it.end; p += skip) {
            if (p + skip > it.end) {
                skip = it.end - p;
            }
            res += *(char *) p;
            len += skip;
            if (len % (uint64_t(1) << 25) == 0) {
                std::cout << std::dec << len / 1024 / 1024 << "MB" << std::endl;
            }
            if (strncmp(it.path,"[vvar]",sizeof("[vvar]"))==0) {
                p += skip;
                std::cout << std::hex << "skip none exists page " << p << "<---->" << it.end << " " << (it.end - p)
                          << " error = " << errno << " desc = " << strerror(errno) << std::endl;
                break;
            }
        }
        std::cout << std::hex << "addr " << p << " skip len " << skip << std::endl;
    }
    len = 0;
    for (auto &&it: vecFree) {
        if(it.start> addrEnd)
        {
            break;
        }
        std::cout << "start travel from " << std::hex << it.start << "<---->" << it.end << " " << it.size << std::endl;
        size_t skip = PAGE_SIZE ;
        auto p = it.start;
        size_t bytes = 0;
        for (; p < it.end; p += skip) {
            if (p + skip > it.end) {
                skip = it.end - p;
            }
            res += *(char *) p;
            munmap((void *) p, skip);
            len += skip;
            bytes += skip;
            if (bytes >= (uint64_t(1) << 36)) {
                std::cout << std::dec << len / 1024 / 1024 / 1024 << "GB" << std::endl;
                bytes = 0;
            }
        }
        std::cout << std::hex << "addr " << p << " skip len " << skip << std::endl;
    }

    auto nEnd = std::chrono::steady_clock::now();

    std::cout << std::dec << "travel cost " << std::chrono::duration_cast<std::chrono::seconds>(nEnd - nStart).count() << std::endl;

    std::cout << res << std::endl;

    std::cout << std::dec << nAllSize / 1024 / 1024 / 1024 / 1024 << "TB" << std::endl;

    // std::cin.get();

    return 0;
}