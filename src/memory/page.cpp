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

#include "utils/rss.h"
#include <unistd.h>
int main(int argc, char **argv)
{
    rusage usage{};
    FillRSS(usage);
    auto sz = size_t(1) << 32;
    auto p  = (char *)sbrk(sz);
    for (size_t i = 0; i < sz / 4096; i++)
    {
        p[0] = 'c';
        p += 4096;
    }
    PrintUsage(usage);
    return 0;
}