//
// Created by Administrator on 2021/9/12.
//

#include <deque>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <list>
#include <memory>
#include <gperftools/malloc_extension.h>
#include <gperftools/malloc_hook.h>
#include <gperftools/heap-profiler.h>
#include <gperftools/heap-checker.h>
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


    std::vector<uint64_t*> vecPtr;
    auto  *p=new uint64_t;
    auto counter=0;
    while(1)
    {
        auto ref=new uint64_t ;
       if((ref-p)>2)
       {
           std::cout<<vecPtr.size()<<'\n';
           std::cout<<(char*)ref-(char*)p<<'\n';
           if(counter++>10)
           {
               break ;
           }
       }
       vecPtr.emplace_back(ref);
       p=ref;
    }
    for(auto &it:vecPtr)
    {
        delete it;
    }
    return 0;
}