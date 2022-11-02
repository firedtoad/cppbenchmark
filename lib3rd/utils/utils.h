//
// Created by Bytedance on 2022/11/2.
//

#ifndef BENCH_UTILS_H
#define BENCH_UTILS_H
#include <iostream>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <thread>

template <class Tp> inline __attribute__((always_inline)) void DoNotOptimize(Tp &value)
{
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

inline uint64_t getThreadRss()
{
    rusage usage{};
    getrusage(RUSAGE_THREAD, &usage);
    return usage.ru_maxrss;
}

inline void FillRSS(rusage &rUsage)
{
    auto rss        = getThreadRss();
    uint64_t newRss = 0;
    int sz          = 1;
    auto p          = (char *)calloc(1, 1);
    while (newRss <= rss)
    {
        getrusage(RUSAGE_THREAD, &rUsage);
        p      = (char *)calloc(sz++, 1);
        newRss = getThreadRss();
        DoNotOptimize(p);
    }
}

inline std::string operator-(const timeval tv1, const timeval tv2)
{
    timeval res;
    timersub(&tv1, &tv2, &res);
    return std::to_string(res.tv_sec) + "s " + std::to_string(res.tv_usec) + "us";
}

inline void printUsage(struct rusage &rUsage)
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

#endif // BENCH_UTILS_H
