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
#include "utils/rss.h"

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