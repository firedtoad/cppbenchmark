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
#include <functional>
#include <gperftools/heap-checker.h>
#include <gperftools/heap-profiler.h>
#include <gperftools/malloc_extension.h>
#include <gperftools/malloc_hook.h>
#include <initializer_list>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include <boost/context/continuation.hpp>
#include <boost/coroutine2/coroutine.hpp>
#include <map>

struct A
{
    virtual void f()
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }

    ~A()
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };
};

struct B : public A
{
    void f() override
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
    ~B()
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };
};

struct C : public B
{
    void f() override
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
    ~C()
    {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };
};

template <class Tp> struct NAlloc
{
    typedef Tp value_type;
    NAlloc() = default;
    template <class T> NAlloc(const NAlloc<T> &) {}
    Tp *allocate(std::size_t n)
    {
        n *= sizeof(Tp);
        std::cout << "allocating " << n << " bytes\n";
        return static_cast<Tp *>(::operator new(n));
    }
    void deallocate(Tp *p, std::size_t n)
    {
        std::cout << "deallocating " << n * sizeof *p << " bytes\n";
        ::operator delete(p);
    }
};

int main(int argc, char **argv)
{
    //    int a[10]{};
    //    int b{20};
    //    A *pa = new C{};
    //    pa->f();
    //    delete pa;
    //
    //    std::cout << "Default-construct deque:\n";
    //    //    std::deque<uint64_t , NAlloc<uint64_t>> deq;
    //    //    std::list<int, NAlloc<int>> list;
    //    //    deq.push_back(1);
    //    std::deque<Entry, NAlloc<Entry>> deq;
    //    //    std::list<Entry,NAlloc<Entry>> queue_;
    //    //    queue_.push_back({});
    //    //    queue_.push_back({});
    //
    //    deq.push_back({});
    //    deq.push_back({});
    //    deq.push_back({});
    //    deq.push_back({});
    //    deq.push_back({});
    //    deq.push_back({});
    //    std::vector<MallocExtension::FreeListInfo> vecInfo;
    //    std::cout<<MallocExtension::instance()->GetMemoryReleaseRate()<<'\n';
    //    MallocExtension::instance()->GetFreeListSizes(&vecInfo);
    //    for(auto &p:vecInfo)
    //    {
    //        std::cout<<p.type<<' '<<p.min_object_size<<' '<<p.max_object_size<<' '<<p.total_bytes_free<<'\n';
    //    }

    std::vector<uint64_t *> vecPtr;
    auto *p      = new uint64_t;
    auto counter = 0;
    while (1)
    {
        auto ref = new uint64_t;
        if ((ref - p) > 2)
        {
            std::cout << vecPtr.size() << '\n';
            std::cout << (char *)ref - (char *)p << '\n';
            if (counter++ > 1)
            {
                break;
            }
        }
        vecPtr.emplace_back(ref);
        p = ref;
    }
    for (auto &it : vecPtr)
    {
        delete it;
    }
    {
        std::vector<int> vi = {1};
        std::vector<int> va = {1, 2, 3, 4, 5, 6, 12, 11, 4, 5, 6, 21, 14};
        std::cout << vi.capacity() << '\n';
        std::cout << va.capacity() << '\n';
        vi.insert(vi.end(), va.begin(), va.end());
        std::cout << vi.capacity() << '\n';
        std::cout << va.capacity() << '\n';
    }

    typedef boost::coroutines2::asymmetric_coroutine<int> coro_t;

    coro_t::pull_type source(
        [&](coro_t::push_type &sink)
        {
            int first = 1, second = 1;
            sink(first);
            sink(second);
            for (int i = 0; i < 8; ++i)
            {
                int third = first + second;
                first     = second;
                second    = third;
                sink(third);
            }
        });

    for (auto i : source)
        std::cout << i << " ";

    {
        namespace ctx = boost::context;
        int a;
        ctx::continuation source = ctx::callcc(
            [&a](ctx::continuation &&sink)
            {
                a     = 0;
                int b = 1;
                for (;;)
                {
                    sink     = sink.resume();
                    int next = a + b;
                    a        = b;
                    b        = next;
                }
                return std::move(sink);
            });
        for (int j = 0; j < 10; ++j)
        {
            std::cout << a << " ";
            source = source.resume();
        }
    }

    {
        std::vector<std::reference_wrapper<int>> v;
        int x;
        v.emplace_back(x);
    }
    {
        struct XX
        {
            char cc[1024];
            int i;
            char c;
            double d;
            float f;
        };

        std::map<int, XX> mx;
        auto x   = mx.insert(std::pair(1, XX{}));
        auto &xx = x.first->second;
        auto &yy = mx[2];
        std::cout << xx.cc[0] << '\n';
        std::cout << yy.cc[0] << '\n';
    }

    return 0;
}