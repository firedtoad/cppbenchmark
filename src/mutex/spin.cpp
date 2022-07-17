#include "benchmark/benchmark.h"
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <absl/base/internal/spinlock.h>

template<typename T>
void benchmark_mutex_lock_unlock(benchmark::State &state)
{
  static T m;
  while (state.KeepRunning())
  {
    m.lock();
    m.unlock();
  }
}

BENCHMARK_TEMPLATE(benchmark_mutex_lock_unlock,boost::detail::spinlock)->ThreadRange(1,16);

template<typename T>
void benchmark_mutex_Lock_Unlock(benchmark::State &state)
{
  static T m(absl::base_internal::SchedulingMode::SCHEDULE_COOPERATIVE_AND_KERNEL);
  while (state.KeepRunning())
  {
    m.Lock();
    m.Unlock();
  }
}

BENCHMARK_TEMPLATE(benchmark_mutex_Lock_Unlock,absl::base_internal::SpinLock)->ThreadRange(1,16);

int main(int argc, char *argv[])
{
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  return 0;
}
