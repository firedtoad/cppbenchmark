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
#include <vector>
//
// void *operator new(size_t size)
//{
//    std::cout << __PRETTY_FUNCTION__ << " size " << size << '\n';
//    void *p = malloc(size);
//    if (!p)
//        throw std::bad_alloc();
//
//    return p;
//}
//
// void *operator new[](size_t size)
//{
//    std::cout << __PRETTY_FUNCTION__ << " size " << size << '\n';
//    void *p = malloc(size);
//    if (!p)
//        throw std::bad_alloc();
//
//    return p;
//}

void operator delete(void *p, size_t size) noexcept
{
    std::cout << __PRETTY_FUNCTION__ << " size " << size << '\n';
    free(p);
}

void operator delete[](void *p, size_t size) noexcept
{
    std::cout << __PRETTY_FUNCTION__ << " size " << size << '\n';
    free(p);
}

void operator delete[](void *p) noexcept
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
    free(p);
}

struct S
{
    char c[1];

    void *operator new[](size_t size)
    {
        std::cout << __PRETTY_FUNCTION__ << " size " << size << '\n';
        return malloc(size);
    }
    void operator delete[](void *p, size_t size)
    {
        std::cout << __PRETTY_FUNCTION__ << " size " << size << '\n';
        operator delete(p);
    }
};

int main(int argc, char **argv)
{
    {
        auto p = new S;
        std::cout << p << '\n';
        delete p;
    }
    {
        auto p = new S[10]{};
        std::cout << p << '\n';
        delete[] p;
    }
    {
        std::vector<S> v;
        v.reserve(10);
        std::cout << v.capacity() << '\n';
    }
    {
        std::vector<S *> v;
        while (1)
        {
            auto n = new S;
            v.emplace_back(n);
            auto sz = v.size();
            if (sz > 2 && (v[sz - 1] - v[sz - 2]) > 4096)
            {
                std::cout << v[sz - 1] << " : " << v[sz - 2] << " diff " << v[sz - 1] - v[sz - 2] << " sz " << sz << '\n';
                break;
            }
        }
        for (auto p : v)
        {
            //            delete p;
        }
    }
}
