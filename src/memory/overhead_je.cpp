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
#include "utils/rss.h"

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