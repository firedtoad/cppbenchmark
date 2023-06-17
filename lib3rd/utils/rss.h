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

#ifndef BENCH_UTILS_H
#define BENCH_UTILS_H
#include <cstring>
#include <iostream>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <vector>

template <class Tp> inline __attribute__((always_inline)) void DoNotOptimize(Tp &&value)
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

#ifdef JE_MALLOC
extern "C" struct mallinfo je_mallinfo();
#define mallinfo je_mallinfo
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
inline void FillRSS(rusage &rUsage)
{
    auto rss        = getThreadRss(rUsage);
    uint64_t newRss = 0;
    int sz          = 1;
    auto p          = (char *)calloc(1, 4096);

    while (newRss <= rss)
    {
        newRss = getThreadRss(rUsage);
        p      = (char *)calloc(++sz, 4096);
        auto s = (char *)((uintptr_t)p & (-4096));
        while (s < (p + sz * 4096))
        {
            *s = '0';
            s += 4096;
        }
        DoNotOptimize(p);
    }

    auto info = mallinfo();
    p         = (char *)calloc(info.fordblks, 1);
    DoNotOptimize(p);
}
#pragma GCC diagnostic pop

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
    std::cout << "user cpu             : " << usage.ru_utime - rUsage.ru_utime << '\n';
    std::cout << "sys cpu              : " << usage.ru_stime - rUsage.ru_stime << '\n';
    std::cout << "max rss              : " << (usage.ru_maxrss - rUsage.ru_maxrss) << " KB / " << (usage.ru_maxrss - rUsage.ru_maxrss) / 1024.0
              << " MB" << '\n';
    std::cout << "page reclaims        : " << usage.ru_minflt - rUsage.ru_minflt << '\n';
    std::cout << "page faults          : " << usage.ru_majflt - rUsage.ru_majflt << '\n';
    std::cout << "swapped out          : " << usage.ru_nswap - rUsage.ru_nswap << '\n';
    std::cout << "voluntary switches   : " << usage.ru_nvcsw - rUsage.ru_nvcsw << '\n';
    std::cout << "involuntary switches : " << usage.ru_nivcsw - rUsage.ru_nivcsw << '\n';
    std::cout << '\n';
    std::cout.flush();
}

#endif // BENCH_UTILS_H
