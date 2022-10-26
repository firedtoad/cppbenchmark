//#include <sys/time.h>
//#include <chrono>
//#include <iostream>
//int main(int argc, char **argv)
//{
//    auto begin=std::chrono::steady_clock::now();
//    auto v=0;
//    for(auto i=0;i<1000000;i++)
//    {
//        timeval tv;
//        v=gettimeofday(&tv, nullptr);
//    }
//    auto end=std::chrono::steady_clock::now();
//    std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>((end-begin)).count()<<"ms\n";
//    return v;
//}

#include <chrono>
#include <iostream>
#include <sys/time.h>

template <class Tp>
inline __attribute__((always_inline)) void DoNotOptimize(Tp& value) {
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}
int main(int argc, char **argv)
{
    auto begin=std::chrono::steady_clock::now();
    auto v=0;
    for(auto i=0;i<1000000;i++)
    {
        auto val=std::chrono::steady_clock::now();
        DoNotOptimize(val);
    }
    auto end=std::chrono::steady_clock::now();
    std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>((end-begin)).count()<<"ms\n";
    return v;
}
