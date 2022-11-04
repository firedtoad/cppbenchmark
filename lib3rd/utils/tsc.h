#ifndef BENCH_TSC_H
#define BENCH_TSC_H
#include <stdint.h>

inline uint64_t hardware_timestamp()
{
#if defined(__GNUC__) && defined (__x86_64__)
    return __builtin_ia32_rdtsc();
#else
    return std::chrono::steady_clock::now().time_since_epoch().count();
#endif
}

inline uint64_t hardware_timestamp_p(uint32_t &ui)
{
#if defined(__GNUC__) && defined (__x86_64__)
    return __builtin_ia32_rdtscp(&ui);
#else
    return std::chrono::steady_clock::now().time_since_epoch().count();
#endif
}
#endif // BENCH_TSC_H
