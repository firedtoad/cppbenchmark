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
