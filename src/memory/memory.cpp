//
// Created by Administrator on 2022/08/11.
//

#include "flat_hash_map.hpp"
#include "tsl/ordered_map.h"
#include "tsl/ordered_set.h"
#include "tsl/robin_map.h"
#include "tsl/robin_set.h"
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

template <class T> class my_allocator
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
            alloc--;
            memAlloc -= n * sizeof(T);
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

int main(int argc, char **argv)
{

    std::cout << sizeof(std::_Rb_tree_node<std::pair<int, bool>>) << '\n';
    std::cout << sizeof(std::__detail::_Hash_node<char, true>) << '\n';
    std::cout << sizeof(__gnu_cxx::__aligned_buffer<char>) << '\n';
    std::cout << sizeof(CC) << '\n';
    {
        std::list<uint32_t, my_allocator<uint32_t>> v;
        v.resize(1024);
        std::cout << "std::list cost memory " << memAlloc << " alloc " << alloc << '\n';
    }

    memAlloc = 0;
    {
        std::deque<uint64_t, my_allocator<uint64_t>> v;
        v.resize(1024);
        std::cout << "std::deque cost memory " << memAlloc << " alloc " << alloc << '\n';
    }

    memAlloc = 0;
    {
        std::vector<uint64_t, my_allocator<uint64_t>> v;
        v.resize(1024);

        std::cout << "std::vector cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    {
        std::map<uint16_t, uint16_t, std::less<uint16_t>, my_allocator<uint16_t>> v;
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "std::map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
    {
        std::set<uint16_t, std::less<uint16_t>, my_allocator<uint16_t>> v;
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "std::set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }

    memAlloc = 0;
    {
        std::unordered_map<uint32_t, uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v[i] = i;
        }
        std::cout << "std::unordered_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }

    memAlloc = 0;
    {
        std::unordered_set<uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "std::unordered_map cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    memAlloc = 0;
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
    {
        tsl::ordered_set<uint32_t, std::hash<uint32_t>, std::equal_to<uint32_t>, my_allocator<uint32_t>> v;
        v.reserve(1024);
        for (auto i = 0; i < 1024; i++)
        {
            v.insert(i);
        }
        std::cout << "tsl::ordered_set cost memory " << memAlloc << " alloc " << alloc << '\n';
    }
    return 0;
}
