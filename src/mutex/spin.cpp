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

#include "benchmark/benchmark.h"
#include <absl/base/internal/spinlock.h>
#include <boost/fiber/detail/futex.hpp>
#include <boost/fiber/detail/spinlock.hpp>
#include <boost/fiber/detail/spinlock_ttas.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>

template <typename T> void benchmark_mutex_lock_unlock(benchmark::State &state)
{
    static T m;
    while (state.KeepRunning())
    {
        m.lock();
        m.unlock();
    }
}

BENCHMARK_TEMPLATE(benchmark_mutex_lock_unlock, boost::detail::spinlock)->ThreadRange(1, 16);
BENCHMARK_TEMPLATE(benchmark_mutex_lock_unlock, std::mutex)->ThreadRange(1, 16);
BENCHMARK_TEMPLATE(benchmark_mutex_lock_unlock, boost::fibers::detail::spinlock_ttas)->ThreadRange(1, 16);

template <typename T> void benchmark_mutex_Lock_Unlock(benchmark::State &state)
{
    static T m(absl::base_internal::SchedulingMode::SCHEDULE_COOPERATIVE_AND_KERNEL);
    while (state.KeepRunning())
    {
        m.Lock();
        m.Unlock();
    }
}

BENCHMARK_TEMPLATE(benchmark_mutex_Lock_Unlock, absl::base_internal::SpinLock)->ThreadRange(1, 16);

int main(int argc, char *argv[])
{
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
