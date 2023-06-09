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
#include "unordered_map.hpp"
#include "utils/symbol.h"
#include <absl/container/btree_map.h>
#include <absl/container/btree_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/container/node_hash_map.h>
#include <absl/container/node_hash_set.h>
#include <benchmark/benchmark.h>
#include <cxxabi.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

template <typename... T> void PrintNode(T &&...t)
{
    (..., (std::cout << demangle(typeid(typename T::node_type).name()) << " size " << sizeof(typename T::node_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintContainer(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(typename T::value_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintList(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(t) << '\n'));
    std::cout << '\n';
}
struct Hasher
{
    size_t operator()(int key) const
    {
        return 1;
    }
};

template <class M> static void BM_unmap(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (int i = 0; i < 10000; i++)
        {
            m[i] = i;
        }
    }
}
template <class M> static void BM_decay_map(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (int i = 0; i < 10000; i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_unmap, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, std::unordered_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, ska::unordered_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, ska::flat_hash_map<int, int>);
// BENCHMARK_TEMPLATE(BM_decay_map, ska::flat_hash_map<int, int,Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, phmap::flat_hash_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, absl::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, absl::flat_hash_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, tsl::ordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, tsl::ordered_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, tsl::vector_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, tsl::vector_map<int, int, Hasher>);

#include "utils/rss.h"

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

    my_allocator() noexcept {}
    my_allocator(const my_allocator &) noexcept {}

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
template <typename F, typename S> struct SS : std::pair<F, S>
{
};

int main(int argc, char **argv)
{
    std::cout << sizeof(SS<uint64_t, char>) << '\n';
    std::cout << sizeof(SS<char, uint64_t>) << '\n';
    std::cout << std::is_trivially_copy_constructible_v<SS<int, char>> << '\n';
    std::cout << sizeof(phmap::priv::hash_policy_traits<phmap::priv::FlatHashSetPolicy<unsigned char>, void>) << '\n';
    rusage rusage;
    FillRSS(rusage);
    PrintNode(absl::btree_set<uint8_t>{}, absl::btree_set<uint16_t>{}, absl::btree_set<uint32_t>{}, absl::btree_set<uint64_t>{});

    absl::btree_set<uint32_t, std::less<>, my_allocator<uint32_t>> bset;
    for (auto i = 0; i < 1024; i++)
    {
        bset.insert(i);
    }

    PrintNode(phmap::flat_hash_set<uint8_t>{}, phmap::flat_hash_set<uint16_t>{}, phmap::flat_hash_set<uint32_t>{}, phmap::flat_hash_set<uint64_t>{});

    PrintNode(absl::flat_hash_set<uint8_t>{}, absl::flat_hash_set<uint16_t>{}, absl::flat_hash_set<uint32_t>{}, absl::flat_hash_set<uint64_t>{});
    PrintNode(absl::node_hash_set<uint8_t>{}, absl::node_hash_set<uint16_t>{}, absl::node_hash_set<uint32_t>{}, absl::node_hash_set<uint64_t>{});

    PrintNode(phmap::flat_hash_map<uint8_t, uint8_t>{}, phmap::flat_hash_map<uint16_t, uint16_t>{}, phmap::flat_hash_map<uint32_t, uint32_t>{},
              phmap::flat_hash_map<uint64_t, uint64_t>{});

    PrintNode(absl::btree_map<uint8_t, uint8_t>{}, absl::btree_map<uint16_t, uint16_t>{}, absl::btree_map<uint32_t, uint32_t>{},
              absl::btree_map<uint64_t, uint64_t>{});

    PrintNode(absl::flat_hash_map<uint8_t, uint8_t>{}, absl::flat_hash_map<uint16_t, uint16_t>{}, absl::flat_hash_map<uint32_t, uint32_t>{},
              absl::flat_hash_map<uint64_t, uint64_t>{});

    PrintNode(absl::node_hash_map<uint8_t, uint8_t>{}, absl::node_hash_map<uint16_t, uint16_t>{}, absl::node_hash_map<uint32_t, uint32_t>{},
              absl::node_hash_map<uint64_t, uint64_t>{});

    PrintContainer(ska::flat_hash_set<uint8_t>{}, ska::flat_hash_set<uint16_t>{}, ska::flat_hash_set<uint32_t>{}, ska::flat_hash_set<uint64_t>{});
    PrintContainer(ska::flat_hash_map<uint8_t, uint8_t>{}, ska::flat_hash_map<uint16_t, uint16_t>{}, ska::flat_hash_map<uint32_t, uint32_t>{},
                   ska::flat_hash_map<uint64_t, uint64_t>{});

    PrintContainer(ska::unordered_set<uint8_t>{}, ska::unordered_set<uint16_t>{}, ska::unordered_set<uint32_t>{}, ska::unordered_set<uint64_t>{});
    PrintContainer(ska::unordered_map<uint8_t, uint8_t>{}, ska::unordered_map<uint16_t, uint16_t>{}, ska::unordered_map<uint32_t, uint32_t>{},
                   ska::unordered_map<uint64_t, uint64_t>{});

    PrintNode(std::unordered_set<uint8_t>{}, std::unordered_set<uint16_t>{}, std::unordered_set<uint32_t>{}, std::unordered_set<uint64_t>{});
    PrintNode(std::unordered_map<uint8_t, uint8_t>{}, std::unordered_map<uint16_t, uint16_t>{}, std::unordered_map<uint32_t, uint32_t>{},
              std::unordered_map<uint64_t, uint64_t>{});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}