//
// Created by dietoad on 2022/11/2.
//


#include <unistd.h>
#include "utils/utils.h"
int main(int argc, char **argv)
{
    rusage usage{};
    FillRSS(usage);
    auto sz = size_t(1) << 20;
    auto p  = (char *)sbrk(sz);
    for (size_t i = 0; i < sz / 4096; i++)
    {
        p[0] = 'c';
        p += 4096;
    }
    printUsage(usage);
    return 0;
}