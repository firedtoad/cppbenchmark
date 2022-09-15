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
int main(int argc, char **argv)
{
    auto begin=std::chrono::steady_clock::now();
    auto v=0;
    for(auto i=0;i<1000000;i++)
    {
        auto now=time(nullptr);
        struct tm tm;
        localtime_r(&now,&tm);
    }
    auto end=std::chrono::steady_clock::now();
    std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>((end-begin)).count()<<"ms\n";
    return v;
}
