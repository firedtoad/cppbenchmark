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
#include <iostream>
#include <setjmp.h>
thread_local  jmp_buf buf{};
thread_local sigset_t _sigset;
void sigsegv(int)
{
    sigprocmask(SIG_UNBLOCK, &_sigset, NULL);
    longjmp(buf, 1);
}
int *p = nullptr;
int *d = new int{0};
static void BM_LongJump(benchmark::State &state)
{
    for (auto _ : state)
    {
        if (setjmp(buf) == 0)
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

BENCHMARK(BM_LongJump);

static void BM_TryCatch(benchmark::State &state)
{

    for (auto _ : state)
    {
        try
        {
            throw d;
//            benchmark::DoNotOptimize(*d);
        }catch (int* ptr)
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

BENCHMARK(BM_TryCatch);


int main(int argc, char **argv)
{
    struct sigaction act
    {
    };
    act.sa_flags   = SA_SIGINFO;
    act.sa_handler = sigsegv;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGSEGV);
    sigaddset(&act.sa_mask, SIGFPE);
    sigaction(SIGSEGV, &act, nullptr);
    sigaction(SIGFPE, &act, nullptr);
    sigemptyset(&_sigset);
    sigaddset(&_sigset, SIGSEGV);
    sigaddset(&_sigset, SIGFPE);
    if (setjmp(buf) == 0)
    {
        *p = 10;
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
        benchmark::DoNotOptimize(p);
    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
        benchmark::DoNotOptimize(buf);
    }

    if (setjmp(buf) == 0)
    {
        *p = 10;
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
        benchmark::DoNotOptimize(p);
    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << '\n';
        benchmark::DoNotOptimize(buf);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}