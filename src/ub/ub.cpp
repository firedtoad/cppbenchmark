//
// Created by Administrator on 2021/9/12.
//

#include <iostream>

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


int main(int argc, char **argv) {
    int a[10]{};
    int b{20};
    A *pa = new C{};
    pa->f();
    delete pa;
    return 0;
}