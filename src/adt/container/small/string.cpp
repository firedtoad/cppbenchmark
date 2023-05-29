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

#include <iostream>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <malloc.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>

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
        DoNotOptimize(p);
    }
    auto info = mallinfo2();
    p         = (char *)calloc(info.fordblks, 1);
    DoNotOptimize(p);
}
std::string operator-(const timeval tv1, const timeval tv2)
{
    timeval res;
    timersub(&tv1, &tv2, &res);
    return std::to_string(res.tv_sec) + "s " + std::to_string(res.tv_usec) + "us";
}
void PrintUsage(struct rusage &rUsage)
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
const int SIZE    = 1024 * 32;
const int SIZE_1M = 1024 * 1024;
int main(int argc, char *argv[])
{
    // {
    //     llvm::SmallVector<llvm::SmallString<40>, SIZE> ssvec;
    //     ssvec.resize(SIZE);
    // }
    rusage usage{};
    FillRSS(usage);
    llvm::SmallVector<std::string, SIZE> svec;
    svec.resize(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        svec[i].resize(15);
    }
    PrintUsage(usage);
    FillRSS(usage);
    llvm::SmallVector<llvm::SmallString<16>, SIZE> ssvec;
    ssvec.resize(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        ssvec[i].resize(16);
    }
    PrintUsage(usage);
    return 0;
}