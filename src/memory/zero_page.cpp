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

std::string GetStatMRaw()
{
    std::ostringstream oss;
    std::fstream fs("/proc/self/statm", std::ios_base::in | std::ios_base::binary);
    oss << fs.rdbuf();
    return oss.str();
}

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

void PrintMMap()
{
    std::fstream fs("/proc/self/maps", std::ios::binary | std::ios::in);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::string line;
    std::cout << ss.str() << '\n';
}

#include <csignal>
#include <setjmp.h>
#include <ucontext.h>
thread_local jmp_buf buf{};
int first = 1;
void sigsegv(int signo, siginfo_t *info, void *context)
{
    //    auto uctx = (ucontext_t *)context;
    //    siglongjmp(buf, 1);
    //    ;

    //    std::cerr << signo << '\n';
    //    std::cerr << info->si_code << '\n';
    //    std::cerr << info->si_errno << '\n';
    if (first)
    {
        auto addr = (void *)((uintptr_t)info->si_addr & (-4096));
        std::cerr << info->si_addr << ' ' << addr << '\n';
        mmap(addr, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
        first = 0;
        return;
    }
    exit(0);
}

int main(int argc, char **argv)
{
    //    struct sigaction act
    //    {
    //    };
    //    act.sa_flags     = SA_SIGINFO | SA_NODEFER;
    //    act.sa_sigaction = sigsegv;
    //    sigemptyset(&act.sa_mask);
    //    //    sigaddset(&act.sa_mask, SIGSEGV);
    //    //    sigaddset(&act.sa_mask, SIGTRAP);
    //    sigaction(SIGSEGV, &act, nullptr);
    //    sigaction(SIGFPE, &act, nullptr);
    //    sigaction(SIGTRAP, &act, nullptr);
    //
    //    PrintMMap();
    //    //        void *p = mmap(0, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    //    //        (void *)p;
    //
    ////    std::cout << strerror(errno) << '\n' << '\n';
    //    char *pChar = (char *)0;
    //    *pChar      = 'a';
    //    std::cout << *pChar << '\n';
    //    PrintMMap();
    //
    struct sigaction act
    {
    };
    act.sa_flags     = SA_SIGINFO;
    act.sa_sigaction = sigsegv;
    // act.sa_sigaction = sigsegvMMap;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGSEGV);
    sigaction(SIGSEGV, &act, nullptr);
    // void *p = mmap(0, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE|PROT_EXEC, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    // std::cout << p << ' ' << strerror(errno) << '\n' << '\n';
    // sleep(10000000);
    char *pChar=0;
    *pChar = 'a';
    std::cout << *pChar << '\n';
    PrintMMap();
    return 0;
}