//
// Created by Administrator on 2022/08/14.
//

#include <benchmark/benchmark.h>
#include <iostream>
#include <llvm/ADT/ImmutableList.h>
#include <llvm/ADT/ImmutableMap.h>
#include <llvm/ADT/ImmutableSet.h>
#include <llvm/ADT/FoldingSet.h>

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 103456789, y = 362436069, z = 521088629;
    unsigned long        t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long _random()
{
    return xorshf96();
}

struct Node
{
    int i;
    void Profile(llvm::FoldingSetNodeID &ID) const {}
};

int main(int argc, char **argv)
{
    {
        llvm::ImmutableMap<int, int>::Factory factory;
        auto imap = factory.getEmptyMap();
        for (auto i = 0; i < 1024; i++)
        {
            imap = factory.add(imap, i, i);
        }
        for (auto &it : imap)
        {
            std::cout << it.first << ":" << it.second << '\n';
        }
    }
    {
        llvm::ImmutableSet<int>::Factory factory;

        auto imset = factory.getEmptySet();
        for (auto i = 0; i < 1024; i++)
        {
            imset = factory.add(imset, i);
        }
        for (auto &it : imset)
        {
            std::cout << it << '\n';
        }
    }
    {
        llvm::ImmutableList<Node>::Factory factory;
        auto                               list = factory.getEmptyList();
        for (auto i = 0; i < 10; i++)
        {
            list = factory.emplace( list,Node{i});
        }
        for(auto &it:list)
        {
            std::cout<<it.i<<'\n';
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}