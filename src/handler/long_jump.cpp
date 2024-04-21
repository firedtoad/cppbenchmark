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

#include <benchmark/benchmark.h>
#include <csignal>
#include <cxxabi.h>
#include <iostream>
#include <memory>
#include <setjmp.h>
#include <unwind.h>

// void
// longjmp_unwind (jmp_buf env, int val)
//{
//     pthread_cleanup (env->__jmpbuf,  __builtin_frame_address(0));
// }

// thread_local jmp_buf buf{};
thread_local sigjmp_buf buf{};

typedef void (*cxa_throw)(void *, std::type_info *, void(_GLIBCXX_CDTOR_CALLABI *)(void *));
cxa_throw my_throw = abi::__cxa_throw;

void sigsegv(int, siginfo_t *, void *)
{
//    auto p     = abi::__cxa_allocate_exception(8);
//    int n      = 0;
//    auto &info = typeid(int);
//    my_throw(&n, const_cast<std::type_info *>(&info), nullptr);
//    auto ptr = abi::__cxa_get_exception_ptr(p);
//    __cxxabiv1::__cxa_begin_catch(ptr);
//    __cxxabiv1::__cxa_end_catch();
//    _Unwind_Resume((_Unwind_Exception *)ptr);
    //    siglongjmp(buf, 1);

    //    throw 1;
asm("\tpushq %rbp\n"
    "\tmovl $4, %edi\n"
    "\tpushq   %rbx\n"
    "\tpushq   %rax\n"
    "\tcall    __cxa_allocate_exception@PLT\n"
    "\tmovq    %rax, %rbx\n"
    "\tmovl    $1, (%rax)\n"
    "\txorl    %edx, %edx\n"
    "\tleaq    0x164f(%rip), %rsi\n"
    "\tmovq    %rax, %rdi\n"
    "\tcall    __cxa_throw@PLT\n"
    "\tendbr64\n"
    "\tmovq    %rax, %rbp\n"
    "\tmovq    %rbx, %rdi\n"
    "\tcall    __cxa_free_exception@PLT\n"
    "\tmovq    %rbp, %rdi\n"
    "\tcall    _Unwind_Resume@PLT"
);
}
int *p = nullptr;
int *d = new int{0};
static void BM_LongJumpNoJump(benchmark::State &state)
{
    for (auto _ : state)
    {
        if (sigsetjmp(buf, 1) == 0)
        {
            benchmark::DoNotOptimize(p);
        }
        else
        {
            benchmark::DoNotOptimize(buf);
        }
    }
}

static void BM_LongJump(benchmark::State &state)
{
    for (auto _ : state)
    {
        if (sigsetjmp(buf, 1) == 0)
        {
            *p = 10;
            benchmark::DoNotOptimize(p);
        }
        else
        {
            benchmark::DoNotOptimize(buf);
        }
    }
}
BENCHMARK(BM_LongJumpNoJump)->ThreadRange(1, 8);
BENCHMARK(BM_LongJump)->ThreadRange(1, 8);

static void BM_TryCatchNoThrow(benchmark::State &state)
{

    for (auto _ : state)
    {
        try
        {
            benchmark::DoNotOptimize(p);
        }
        catch (int *ptr)
        {
            benchmark::DoNotOptimize(ptr);
        }
        catch (std::exception &e)
        {
            benchmark::DoNotOptimize(e);
        }
        catch (...)
        {
        }
    }
}

static void BM_TryCatch(benchmark::State &state)
{
    for (auto _ : state)
    {
        try
        {
            throw d;
        }
        catch (int *ptr)
        {
            benchmark::DoNotOptimize(ptr);
        }
        catch (std::exception &e)
        {
            benchmark::DoNotOptimize(e);
        }
        catch (...)
        {
        }
    }
}

BENCHMARK(BM_TryCatchNoThrow)->ThreadRange(1, 8);
BENCHMARK(BM_TryCatch)->ThreadRange(1, 8);
struct AA
{
    ~AA()
    {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
    }
};
struct CC
{
    ~CC()
    {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
    }
    AA c;
    //    std::unique_ptr<std::string> s1 = std::make_unique<std::string>("12345678901234567890");
};

int main(int argc, char **argv)
{
    struct sigaction act
    {
    };
    act.sa_flags   = SA_SIGINFO | SA_NODEFER;
    act.sa_sigaction = sigsegv;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGSEGV);
    sigaction(SIGSEGV, &act, nullptr);
    sigaction(SIGFPE, &act, nullptr);
    //    CC cc;
    try
    {

        if (sigsetjmp(buf, 1) == 0)
        {
            []()
            {
                CC cc;
                *p = 10;
                std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
            }();
            benchmark::DoNotOptimize(p);
        }
        else
        {
            std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
            benchmark::DoNotOptimize(buf);
        }
        if (sigsetjmp(buf, 1) == 0)
        {
            //        auto p1 = std::make_shared<std::string>("1234567890123456789");
            *p = 10;
            std::cout << __PRETTY_FUNCTION__ << __LINE__ << p << '\n';
            benchmark::DoNotOptimize(p);
        }
        else
        {
            std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
            benchmark::DoNotOptimize(buf);
        }
    }
    catch (...)
    {
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}