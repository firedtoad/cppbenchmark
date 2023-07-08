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

#include "utils/rss.h"
#include <fstream>
#include <sstream>
#include <sys/mman.h>
#include <unistd.h>

const int PAGE_SIZE = sysconf(_SC_PAGESIZE);

inline std::string &trim_(std::string &str)
{
    const char *spaces = " \n\r\t";
    str.erase(str.find_last_not_of(spaces) + 1);
    str.erase(0, str.find_first_not_of(spaces));
    return str;
}

static inline void StringSplit(const std::string &str, const std::string &delimiters, std::vector<std::string> &elems)
{
    std::string::size_type pos, prev = 0;
    while ((pos = str.find_first_of(delimiters, prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            elems.emplace_back(str, prev, pos - prev);
        }
        prev = pos + 1;
    }
    if (prev < str.size())
        elems.emplace_back(str, prev, str.size() - prev);
}

struct stat_m
{
    size_t vmSize;
    size_t vmRss;
    size_t vmShare;
    size_t textSize;
    size_t libSize;
    size_t dataSize;
    size_t dirtyPages;
};

stat_m GetStatM()
{
    std::ostringstream oss;
    std::fstream fs("/proc/self/statm", std::ios_base::in | std::ios_base::binary);
    oss << fs.rdbuf();
    std::vector<std::string> vec;
    StringSplit(oss.str(), " ", vec);
    stat_m ret;
    if (vec.size() >= 7)
    {
        size_t *p = &ret.vmSize;
        for (const auto &it : vec)
        {
            if (p <= &ret.dirtyPages)
            {
                *p++ = std::stoul(it) * PAGE_SIZE;
            }
        }
    }
    return ret;
}

void PrintStatM(const stat_m &m)
{
    std::cout << "vmSize    : " << m.vmSize << " B / " << m.vmSize / 1024 << " KB /" << m.vmSize / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << "vmRss     : " << m.vmRss << " B / " << m.vmRss / 1024 << " KB /" << m.vmRss / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << "vmShare   : " << m.vmShare << " B / " << m.vmShare / 1024 << " KB /" << m.vmShare / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << "textSize  : " << m.textSize << " B / " << m.textSize / 1024 << " KB /" << m.textSize / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << "libSize   : " << m.libSize << " B / " << m.libSize / 1024 << " KB /" << m.libSize / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << "dataSize  : " << m.dataSize << " B / " << m.dataSize / 1024 << " KB /" << m.dataSize / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << "dirtySize : " << m.dirtyPages << " B / " << m.dirtyPages / 1024 << " KB /" << m.dirtyPages / 1024.0 / 1024.0 << " MB" << '\n';
    std::cout << '\n';
    std::cout.flush();
}

int main(int argc, char **argv)
{
    auto ret = GetStatM();
    PrintStatM(ret);
    rusage usage{};
    FillRSS(usage);
    auto sz    = size_t(1) << 32;
    auto p     = (char *)sbrk(sz);
    auto old_p = p;
    for (size_t i = 0; i < sz / 4096; i++)
    {
        p[0] = 'c';
        p += 4096;
    }
    PrintUsage(usage);
    ret = GetStatM();
    PrintStatM(ret);
    auto r = madvise(old_p, sz, MADV_DONTNEED);
    std::cout << r << ' ' << strerror(errno) << '\n';
    PrintUsage(usage);
    ret = GetStatM();
    PrintStatM(ret);
    return 0;
}