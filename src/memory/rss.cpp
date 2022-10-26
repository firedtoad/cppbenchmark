#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sys/resource.h>
#include <thread>
#include <unistd.h>
#include <vector>
void printUsage(struct rusage &rUsage)
{
    rusage usage{};
    getrusage(RUSAGE_THREAD, &usage);
    std::cout << "user cpu : " << usage.ru_utime.tv_usec - rUsage.ru_utime.tv_usec << '\n';
    std::cout << "sys cpu : " << usage.ru_stime.tv_usec - rUsage.ru_stime.tv_usec << '\n';
    std::cout << "max rss : " << (usage.ru_maxrss - rUsage.ru_maxrss) / 1024 << " mb" << '\n';
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
    std::thread th_vector_u8(
        []
        {
            auto pSize = sysconf(_SC_PAGESIZE);
            rusage usage{};
            getrusage(RUSAGE_THREAD, &usage);
            std::vector<uint8_t> vec;
            vec.resize(SIZE);
            std::cout << "vector of u8 " << SIZE << '\n';
            std::cout << SIZE / pSize << '\n';
            printUsage(usage);
        });
    std::thread th_vector_u64(
        []
        {
            rusage usage{};
            getrusage(RUSAGE_THREAD, &usage);
            std::vector<uint64_t> vec;
            vec.resize(SIZE);
            std::cout << "vector of u64 " << SIZE << '\n';
            printUsage(usage);
        });
    std::thread th_list_u8(
        []
        {
            rusage usage{};
            getrusage(RUSAGE_THREAD, &usage);
            std::list<uint8_t> li;
            li.resize(SIZE);
            std::cout << "list of u8 " << SIZE << '\n';
            printUsage(usage);
        });
    std::thread th_list_u64(
        []
        {
            rusage usage{};
            getrusage(RUSAGE_THREAD, &usage);
            std::list<uint64_t> li;
            li.resize(SIZE);
            std::cout << "list of u64 " << SIZE << '\n';
            printUsage(usage);
        });
    std::thread th_set_u8(
        []
        {
            rusage usage{};
            getrusage(RUSAGE_THREAD, &usage);
            std::set<uint8_t> li;
            for (auto i = 0; i < SIZE; i++)
            {
                li.insert(i);
            }
            std::cout << "set of u8 " << SIZE << '\n';
            printUsage(usage);
        });
    std::thread th_set_u64(
        []
        {
            rusage usage{};
            getrusage(RUSAGE_THREAD, &usage);
            std::set<uint64_t> li;
            for (auto i = 0; i < SIZE; i++)
            {
                li.insert(i);
            }
            std::cout << "set of u64 " << SIZE << '\n';
            printUsage(usage);
        });
    th_vector_u8.detach();
    th_vector_u64.detach();
    th_list_u8.detach();
    th_list_u64.detach();
    th_set_u8.detach();
    th_set_u64.join();
    return 0;
}
