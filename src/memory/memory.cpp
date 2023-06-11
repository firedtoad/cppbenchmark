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

#include "flat_hash_map.hpp"
#include "parallel_hashmap/phmap.h"
#include "tsl/ordered_map.h"
#include "tsl/ordered_set.h"
#include "tsl/robin_map.h"
#include "tsl/robin_set.h"
#include "utils/alloc.h"
#include "utils/symbol.h"
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static size_t memAlloc;
static size_t alloc;

template <class T> class my_allocator : public std::allocator<T>
{
  public:
    typedef size_t size_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T value_type;

    my_allocator() {}
    my_allocator(const my_allocator &) {}

    pointer allocate(size_type n, const void * = 0)
    {
        memAlloc += n * sizeof(T);
        alloc++;
        T *t = (T *)malloc(n * sizeof(T));
        return t;
    }

    void deallocate(void *p, size_type n)
    {
        if (p)
        {
            free(p);
        }
    }

    pointer address(reference x) const
    {
        return &x;
    }
    const_pointer address(const_reference x) const
    {
        return &x;
    }
    my_allocator<T> &operator=(const my_allocator &)
    {
        return *this;
    }
    void construct(pointer p)
    {
        new ((T *)p) T();
    }
    void destroy(pointer p)
    {
        p->~T();
    }

    size_type max_size() const
    {
        return size_t(-1);
    }

    template <class U> struct rebind
    {
        typedef my_allocator<U> other;
    };

    template <class U> my_allocator(const my_allocator<U> &) {}

    template <class U> my_allocator &operator=(const my_allocator<U> &)
    {
        return *this;
    }
};

struct CC
{
    void *p;
    size_t code;
    char c;
};

template <typename... T> void PrintSize(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(t) << '\n'));
}

int main(int argc, char **argv)
{

    auto p=std::make_shared<int>(1);

    PrintSize(std::_List_node<char>{});
    PrintSize(std::_List_node<uint64_t>{});
    PrintSize(std::_List_node<std::shared_ptr<char>>{});
    PrintSize(std::_List_node<std::shared_ptr<uint64_t>>{});
    PrintSize(std::_Rb_tree_node<std::pair<char, char>>{});
    PrintSize(std::_Rb_tree_node<std::pair<int, int>>{});
    PrintSize(std::__detail::_Hash_node<char, false>{});
    PrintSize(std::__detail::_Hash_node<uint64_t, false>{});
    std::allocator<uint8_t> allocator;
    PrintSize(std::_Sp_counted_ptr_inplace<uint8_t,std::allocator<uint8_t>,__gnu_cxx::_S_atomic>{allocator});


    PrintSize(__gnu_cxx::__aligned_buffer<char>{});
    PrintSize(__gnu_cxx::__aligned_buffer<uint64_t>{});
    PrintSize(phmap::flat_hash_set<char>::node_type{});
    PrintSize(phmap::flat_hash_set<uint64_t>::node_type{});

    PrintSize(CC{});

    {
        AllocReport report{"std::vector<uint32_t>"};
//        printf("hello world");
//        std::list<uint32_t> v(1024*1024);

//        v.resize(1024*1024);
    }
    memAlloc = 0;
    alloc    = 0;
    {
        std::list<uint32_t, my_allocator<uint32_t>> v;
        v.resize(1024);
        std::cout << "std::list cost memory " << memAlloc << " alloc " << alloc << '\n';
    }

    memAlloc = 0;
    alloc    = 0;
    {
        std::deque<uint64_t, my_allocator<uint64_t>> v;
        v.resize(1024);
        std::cout << "std::deque cost memory " << memAlloc << " alloc " << alloc << '\n';
    }

    memAlloc = 0;
    alloc    = 0;
    {
        std::vector<uint64_t, my_allocator<uint64_t>> v;
        v.resize(1024);

        std::cout << "std::vector cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        std::map<uint16_t, uint16_t, std::less<uint16_t>, my_allocator<uint16_t>> v;
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "std::map u16 cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        std::map<uint64_t, uint64_t, std::less<uint64_t>, my_allocator<uint64_t>> v;
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "std::map u64 cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        std::set<uint32_t, std::less<uint32_t>, my_allocator<uint32_t>> v;
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "std::set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        std::unordered_set<uint64_t, std::hash<uint64_t>, std::equal_to<uint64_t>, my_allocator<uint64_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "std::unordered_set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        std::unordered_map<uint32_t, uint64_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "std::unordered_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        tsl::robin_map<uint32_t, uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "tsl::robin_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        tsl::robin_set<uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "tsl::robin_set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        ska::flat_hash_map<uint32_t, uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "ska::flat_hash_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        ska::flat_hash_set<uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "ska::flat_hash_set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        tsl::ordered_map<uint32_t, uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<std::pair<uint32_t, uint32_t>>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "tsl::ordered_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        tsl::ordered_set<uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "tsl::ordered_set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        phmap::flat_hash_map<uint32_t, uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<std::pair<uint32_t, uint32_t>>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "phmap::flat_hash_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    alloc    = 0;
    {
        phmap::flat_hash_set<uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "phmap::flat_hash_set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    return 0;
}
