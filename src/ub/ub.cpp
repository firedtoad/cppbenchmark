//
// Created by Administrator on 2021/9/12.
//

#include <iostream>
#include <deque>
#include <list>
#include <memory>
#include <functional>
#include "my_init_list.h"
#include <initializer_list>
struct A {
    virtual void f() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }

     ~A() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };
};

struct B : public A {
    void f() override {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
    ~B() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };
};

struct C : public B {
    void f() override {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
    ~C() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };
};

template <class Tp>
struct NAlloc {
    typedef Tp value_type;
    NAlloc() = default;
    template <class T> NAlloc(const NAlloc<T>&) {}
    Tp* allocate(std::size_t n)
    {
        n *= sizeof(Tp);
        std::cout << "allocating " << n << " bytes\n";
        return static_cast<Tp*>(::operator new(n));
    }
    void deallocate(Tp* p, std::size_t n)
    {
        std::cout << "deallocating " << n*sizeof*p << " bytes\n";
        ::operator delete(p);
    }
};
template <class T, class U>
bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }

// 挂起标识
struct SuspendEntry {
    std::weak_ptr<uint64_t> tk_;
    uint64_t id_;

    explicit operator bool() const { return tk_.lock()!= nullptr; }
};

// 性能高于LFLock2
struct LFLock
{
    std::atomic_flag flag;

    LFLock() : flag{false}
    {
    }

     void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire)) ;
    }

     bool try_lock()
    {
        return !flag.test_and_set(std::memory_order_acquire);
    }

     void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};


struct Entry
{
    typedef std::function<void()> Func;
    SuspendEntry suspendEntry;

    // 控制是否超时的标志位
    std::shared_ptr<LFLock> noTimeoutLock;

    // 唤醒成功后, 在唤醒线程做的事情
    Func onWakeup;

    Entry() : noTimeoutLock(std::make_shared<LFLock>()) {}
};



int main(int argc, char **argv) {
    int a[10]{};
    int b{20};
    A *pa = new C{};
    pa->f();
    delete pa;

    std::cout << "Default-construct deque:\n";
//    std::deque<uint64_t , NAlloc<uint64_t>> deq;
//    std::list<int, NAlloc<int>> list;
//    deq.push_back(1);
    std::deque<Entry , NAlloc<Entry>> deq;
//    std::list<Entry,NAlloc<Entry>> queue_;
//    queue_.push_back({});
//    queue_.push_back({});

    deq.push_back({});
    deq.push_back({});
    deq.push_back({});
    deq.push_back({});
    deq.push_back({});
    deq.push_back({});
    return 0;
}