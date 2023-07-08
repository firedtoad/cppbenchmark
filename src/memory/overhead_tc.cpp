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
#include <malloc.h>
int main(int argc, char **argv)
{
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
    } while (sz <= 1 << 25);

    return 0;
}