#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sys/resource.h>
#include <thread>
#include <unistd.h>
#include <vector>
const int PAGE_SIZE = sysconf(_SC_PAGESIZE);

template <class Tp> inline __attribute__((always_inline)) void DoNotOptimize(Tp &value)
{
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}
uint64_t getThreadRss()
{
    rusage usage{};
    getrusage(RUSAGE_THREAD, &usage);
    return usage.__ru_maxrss_word;
}

void printUsage(struct rusage &rUsage)
{
    rusage usage{};
    getrusage(RUSAGE_THREAD, &usage);
    std::cout << "user cpu : " << usage.ru_utime.tv_usec - rUsage.ru_utime.tv_usec << '\n';
    std::cout << "sys cpu : " << usage.ru_stime.tv_usec - rUsage.ru_stime.tv_usec << '\n';
    std::cout << "max rss : " << (usage.ru_maxrss - rUsage.ru_maxrss) << " kb / " << (usage.ru_maxrss - rUsage.ru_maxrss) / 1024.0 << " MB" << '\n';
    std::cout << "page reclaims : " << usage.ru_minflt - rUsage.ru_minflt << '\n';
    std::cout << "page faults : " << usage.ru_majflt - rUsage.ru_majflt << '\n';
    std::cout << "voluntary switches : " << usage.ru_nvcsw - rUsage.ru_nvcsw << '\n';
    std::cout << "involuntary switches : " << usage.ru_nivcsw - rUsage.ru_nivcsw << '\n';
    std::cout << '\n';
    std::cout.flush();
}

void printRSS()
{
    int vSize = 0, resident = 0, share = 0;
    std::ifstream buffer("/proc/self/statm");
    buffer >> vSize >> resident >> share;
    buffer.close();
    std::cout << "virtual " << vSize << " rss " << resident << " share " << share << '\n';
}

const int SIZE = 1024 * 1024;

int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;
    //    std::thread th_vector_u8(
    //        []
    //        {
    //            auto pSize = sysconf(_SC_PAGESIZE);
    //            rusage usage{};
    //            getrusage(RUSAGE_THREAD, &usage);
    //            std::vector<uint8_t> vec;
    //            vec.resize(SIZE);
    //            std::cout << "vector of u8 " << SIZE << '\n';
    //            std::cout << SIZE / pSize << '\n';
    //            printUsage(usage);
    //        });
    //    std::thread th_vector_u64(
    //        []
    //        {
    //            rusage usage{};
    //            getrusage(RUSAGE_THREAD, &usage);
    //            std::vector<uint64_t> vec;
    //            vec.resize(SIZE);
    //            std::cout << "vector of u64 " << SIZE << '\n';
    //            printUsage(usage);
    //        });
    //    std::thread th_list_u8(
    //        []
    //        {
    //            rusage usage{};
    //            getrusage(RUSAGE_THREAD, &usage);
    //            std::list<uint8_t> li;
    //            li.resize(SIZE);
    //            std::cout << "list of u8 " << SIZE << '\n';
    //            printUsage(usage);
    //        });
    //    std::thread th_list_u64(
    //        []
    //        {
    //            rusage usage{};
    //            getrusage(RUSAGE_THREAD, &usage);
    //            std::list<uint64_t> li;
    //            li.resize(SIZE);
    //            std::cout << "list of u64 " << SIZE << '\n';
    //            printUsage(usage);
    //        });
    //    std::thread th_set_u8(
    //        []
    //        {
    //            rusage usage{};
    //            getrusage(RUSAGE_THREAD, &usage);
    //            std::set<uint8_t> li;
    //            for (auto i = 0; i < SIZE; i++)
    //            {
    //                li.insert(i);
    //            }
    //            std::cout << "set of u8 " << SIZE << '\n';
    //            printUsage(usage);
    //        });
    //    std::thread th_set_u64(
    //        []
    //        {
    //            rusage usage{};
    //            getrusage(RUSAGE_THREAD, &usage);
    //            std::set<uint64_t> li;
    //            for (auto i = 0; i < SIZE; i++)
    //            {
    //                li.insert(i);
    //            }
    //            std::cout << "set of u64 " << SIZE << '\n';
    //            printUsage(usage);
    //        });
    //    th_vector_u8.detach();
    //    th_vector_u64.detach();
    //    th_list_u8.detach();
    //    th_list_u64.detach();
    //    th_set_u8.detach();
    //    th_set_u64.join();

    auto rss        = getThreadRss();
    uint64_t newRss = 0;
    rusage usage{};
    auto p = (char *)malloc(0);
    auto s = (char *)sbrk(0);
    auto e = (char *)sbrk(0);
    while (newRss <= rss)
    {
        getrusage(RUSAGE_THREAD, &usage);
        p      = (char *)malloc(1);
        *p     = 1;
        e      = (char *)sbrk(0);
        newRss = getThreadRss();
        DoNotOptimize(p);
    }
    std::cout << "diff " << std::dec << e - p << '\n';
    std::cout << "diff " << e - s << '\n';
    std::cout << "diff rss " << newRss - rss << '\n';
    free(p);
    std::cout << std::dec << '\n';
    std::vector<uint64_t> vec;
    vec.resize(SIZE);
    std::cout << "vector of u8 " << SIZE  <<" node size "<< sizeof(std::_List_node<uint8_t>) << '\n';
    printUsage(usage);
    return 0;
}
