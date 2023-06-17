// Copyright 2023 The Division Authors.
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
#include <memory>
template <class Tp> inline __attribute__((always_inline)) void DoNotOptimize(Tp &value)
{
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
    std::cout << &value << '\n';
}
class Base
{
  public:
    ~Base()
    {
        std::cout << __FUNCTION__ << '\n';
    }
    virtual void f(){};
};

class Derived : public Base
{
  public:
    ~Derived()
    {
        std::cout << __FUNCTION__ << '\n';
    }
    std::shared_ptr<Base> p;

  private:
    std::string ss = "123456789012345x";
};

class Derived1 : public Base, public std::enable_shared_from_this<Derived1>
{
  public:
    ~Derived1()
    {
        std::cout << __FUNCTION__ << '\n';
    }
    std::shared_ptr<Base> p;

  private:
    std::string ss = "123456789012345x";
};

std::shared_ptr<Base> p;

void resetPointer(Base *pBase)
{
    p.reset(pBase);
}

void case1()
{
    resetPointer(new Derived);
    std::cout << p << '\n';
}

void case2()
{
    auto p1 = std::make_shared<Derived>();
    p1->p   = p1;
    DoNotOptimize(*p1);
}

void case3()
{
    auto p1 = std::make_shared<Derived>();
    auto p2 = std::make_shared<Derived1>();
    p1->p   = p2;
    p2->p   = p1;
    DoNotOptimize(*p1);
    DoNotOptimize(*p2);
}

int main(int argc, char *argv[])
{
    case1();
    case2();
    case3();
    return 0;
}
