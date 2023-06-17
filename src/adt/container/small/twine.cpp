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

const int SIZE_1M = 1024 * 1024;
int main(int argc, char *argv[])
{
    rusage usage{};
    FillRSS(usage);
    std::string s;
    s.resize(SIZE_1M);
    std::string s1;
    s1.resize(SIZE_1M);
    std::string s2 = s + s1;
    PrintUsage(usage);
    DoNotOptimize(s2);
    FillRSS(usage);
    std::string s3;
    s3.resize(SIZE_1M);
    std::string s4;
    s4.resize(SIZE_1M);
    llvm::Twine t1 = s3;
    llvm::Twine t2 = s4;
    llvm::Twine t3 = t1 + t2;
    PrintUsage(usage);
    DoNotOptimize(t3);
}