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
#undef __STRICT_ANSI__
#include <benchmark/benchmark.h>
#include <deque>
#include <set>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory_resource>
#include <vector>
#include <numeric>

template <class T, size_t N> class my_allocator
{
    T data[N];
    size_t index{};

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
        if (index < N)
        {
            auto pos = index;
            index += n;
            return &data[pos];
        }
        T *t = (T *)malloc(n * sizeof(T));
        return t;
    }

    void deallocate(void *p, size_type n)
    {
        if (p >= &data[0] && p < &data[N])
        {
            return;
        }
        free(p);
    }

    pointer address(reference x) const
    {
        return &x;
    }
    const_pointer address(const_reference x) const
    {
        return &x;
    }
    my_allocator<T,N> &operator=(const my_allocator &)
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
        typedef my_allocator<U, N> other;
    };

    template <class U> my_allocator(const my_allocator<U, N> &) {}

    template <class U> my_allocator &operator=(const my_allocator<U, N> &)
    {
        return *this;
    }
};

template<typename C>
static void BenchListInsert(benchmark::State &state)
{
    for (auto _ : state)
    {
        C  v;
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}
template<typename C>
static void BenchPmrListInsert(benchmark::State &state)
{
    std::vector<uint8_t> vec(state.range(0)*32);
    for (auto _ : state)
    {
        std::pmr::monotonic_buffer_resource pool(vec.data(),vec.size());
        C  v{&pool};
        for (auto i = 0; i < state.range(0); i++)
        {
            v.push_back({});
        }
    }
}

BENCHMARK_TEMPLATE(BenchListInsert,std::list<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchListInsert,std::list<int,my_allocator<std::_List_node<int>,65536>>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchPmrListInsert,std::pmr::list<int>)->Range(1, 65536);

template<typename C>
static void BenchListRange(benchmark::State &state)
{
    C  v;
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back({});
    }
    for (auto _ : state)
    {
        auto r=std::accumulate(v.begin(),v.end(),0);
        benchmark::DoNotOptimize(r);
    }
}

template<typename C>
static void BenchPmrListRange(benchmark::State &state)
{
    std::vector<uint8_t> vec(state.range(0)*32);
    std::pmr::monotonic_buffer_resource pool(vec.data(),vec.size());
    C  v{&pool};
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back({});
    }
    for (auto _ : state)
    {
        auto r=std::accumulate(v.begin(),v.end(),0);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchListRange,std::list<int>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchListRange,std::list<int,my_allocator<std::_List_node<int>,65536>>)->Range(1, 65536);
BENCHMARK_TEMPLATE(BenchPmrListRange,std::pmr::list<int>)->Range(1, 65536);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}