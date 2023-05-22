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
#include <jemalloc/jemalloc.h>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stdint.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <vector>


struct mallinfo
{
    int arena;    /* non-mmapped space allocated from system */
    int ordblks;  /* number of free chunks */
    int smblks;   /* number of fastbin blocks */
    int hblks;    /* number of mmapped regions */
    int hblkhd;   /* space in mmapped regions */
    int usmblks;  /* always 0, preserved for backwards compatibility */
    int fsmblks;  /* space available in freed fastbin blocks */
    int uordblks; /* total allocated space */
    int fordblks; /* total free space */
    int keepcost; /* top-most, releasable (via malloc_trim) space */
};

//extern "C" struct mallinfo je_mallinfo();

//#define mallinfo je_mallinfo

template <class Tp> inline __attribute__((always_inline)) void DoNotOptimize(Tp &value)
{
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

inline uint64_t getThreadRss(rusage &rUsage)
{
    getrusage(RUSAGE_THREAD, &rUsage);
    return rUsage.ru_maxrss;
}

inline void FillRSS(rusage &rUsage)
{
    auto rss        = getThreadRss(rUsage);
    uint64_t newRss = 0;
    int sz          = 1;
    auto p          = (char *)calloc(1, 4096);
    while (newRss <= rss)
    {
        newRss = getThreadRss(rUsage);
        p      = (char *)calloc(sz, 4096);
        pthread_yield();
        DoNotOptimize(p);
    }

    auto info = mallinfo();
    p         = (char *)calloc(info.fordblks, 1);
    DoNotOptimize(p);
}

inline std::string operator-(const timeval tv1, const timeval tv2)
{
    timeval res;
    timersub(&tv1, &tv2, &res);
    return std::to_string(res.tv_sec) + "s " + std::to_string(res.tv_usec) + "us";
}

inline void PrintUsage(struct rusage &rUsage)
{
    rusage usage{};
    getrusage(RUSAGE_THREAD, &usage);
    std::cout << "user cpu : " << usage.ru_utime - rUsage.ru_utime << '\n';
    std::cout << "sys cpu : " << usage.ru_stime - rUsage.ru_stime << '\n';
    std::cout << "max rss : " << (usage.ru_maxrss - rUsage.ru_maxrss) << " kb / " << (usage.ru_maxrss - rUsage.ru_maxrss) / 1024.0 << " MB" << '\n';
    std::cout << "page reclaims : " << usage.ru_minflt - rUsage.ru_minflt << '\n';
    std::cout << "page faults : " << usage.ru_majflt - rUsage.ru_majflt << '\n';
    std::cout << "voluntary switches : " << usage.ru_nvcsw - rUsage.ru_nvcsw << '\n';
    std::cout << "involuntary switches : " << usage.ru_nivcsw - rUsage.ru_nivcsw << '\n';
    std::cout << '\n';
    std::cout.flush();
}
const int SIZE = 1024 * 1024;
int main(int argc, char **argv)
{
    EPERM;
    char *buff;
    size_t l{20};
    int r = mallctl("opt.dss", &buff, &l, nullptr, 0);
    std::cout << r << l << buff << '\n';
    {
        //        malloc_stats_print([](void *p, const char *s){
        //            std::cout<<p<<":"<<s<<'\n';
        //        }, nullptr,"Jgmdablx");
    }

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

    auto sz = 0;
    do
    {
        auto p    = (char *)malloc(sz + 1);
        auto need = sz + 1;
        sz        = malloc_usable_size(p);
        std::cout << "need " << need << " real " << sz << " over head " << sz - need << '\n';
        p    = (char *)malloc(sz);
        need = sz;
        sz   = malloc_usable_size(p);
        std::cout << "need " << need << " real " << sz << " over head " << sz - need << '\n';
    } while (sz <= 1024);
    return 0;
}