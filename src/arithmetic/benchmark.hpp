#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_
#include <stdint.h>
#include <time.h>
#ifdef __x86_64__

const char *unitname = "cycles";

#define RDTSC_START(cycles)                                                                                                                          \
    do                                                                                                                                               \
    {                                                                                                                                                \
        uint32_t cyc_high, cyc_low;                                                                                                                  \
        __asm volatile("cpuid\n"                                                                                                                     \
                       "rdtsc\n"                                                                                                                     \
                       "mov %%edx, %0\n"                                                                                                             \
                       "mov %%eax, %1"                                                                                                               \
                       : "=r"(cyc_high), "=r"(cyc_low)                                                                                               \
                       :                                                                                                                             \
                       :                              /* no read only */                                                                             \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */                                                                                 \
        );                                                                                                                                           \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                                                                                             \
    } while (0)

#define RDTSC_STOP(cycles)                                                                                                                           \
    do                                                                                                                                               \
    {                                                                                                                                                \
        uint32_t cyc_high, cyc_low;                                                                                                                  \
        __asm volatile("rdtscp\n"                                                                                                                    \
                       "mov %%edx, %0\n"                                                                                                             \
                       "mov %%eax, %1\n"                                                                                                             \
                       "cpuid"                                                                                                                       \
                       : "=r"(cyc_high), "=r"(cyc_low)                                                                                               \
                       :                                /* no read only registers */                                                                 \
                       : "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */                                                                               \
        );                                                                                                                                           \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                                                                                             \
    } while (0)

#else
const char *unitname = " (clock units) ";

#define RDTSC_START(cycles)                                                                                                                          \
    do                                                                                                                                               \
    {                                                                                                                                                \
        cycles = clock();                                                                                                                            \
    } while (0)

#define RDTSC_STOP(cycles)                                                                                                                           \
    do                                                                                                                                               \
    {                                                                                                                                                \
        cycles = clock();                                                                                                                            \
    } while (0)
#endif

static __attribute__((noinline)) uint64_t rdtsc_overhead_func(uint64_t dummy)
{
    return dummy;
}

uint64_t global_rdtsc_overhead = (uint64_t)UINT64_MAX;

#define RDTSC_SET_OVERHEAD(test, repeat)                                                                                                             \
    do                                                                                                                                               \
    {                                                                                                                                                \
        uint64_t cycles_start, cycles_final, cycles_diff;                                                                                            \
        uint64_t min_diff = UINT64_MAX;                                                                                                              \
        for (int i = 0; i < repeat; i++)                                                                                                             \
        {                                                                                                                                            \
            __asm volatile("" ::: /* pretend to clobber */ "memory");                                                                                \
            RDTSC_START(cycles_start);                                                                                                               \
            test;                                                                                                                                    \
            RDTSC_STOP(cycles_final);                                                                                                                \
            cycles_diff = (cycles_final - cycles_start);                                                                                             \
            if (cycles_diff < min_diff)                                                                                                              \
                min_diff = cycles_diff;                                                                                                              \
        }                                                                                                                                            \
        global_rdtsc_overhead = min_diff;                                                                                                            \
        printf("rdtsc_overhead set to %d\n", (int)global_rdtsc_overhead);                                                                            \
    } while (0)

/*
 * Prints the best number of operations per cycle where
 * test is the function call, answer is the expected answer generated by
 * test, repeat is the number of times we should repeat and size is the
 * number of operations represented by test.
 */
#define BEST_TIME(test, expected, pre, repeat, size, verbose)                                                                                        \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (global_rdtsc_overhead == UINT64_MAX)                                                                                                     \
        {                                                                                                                                            \
            RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);                                                                                      \
        }                                                                                                                                            \
        if (verbose)                                                                                                                                 \
            printf("%-60s\t: ", #test);                                                                                                              \
        fflush(NULL);                                                                                                                                \
        uint64_t cycles_start, cycles_final, cycles_diff;                                                                                            \
        uint64_t min_diff = (uint64_t)-1;                                                                                                            \
        uint64_t sum_diff = 0;                                                                                                                       \
        for (int i = 0; i < repeat; i++)                                                                                                             \
        {                                                                                                                                            \
            pre;                                                                                                                                     \
            __asm volatile("" ::: /* pretend to clobber */ "memory");                                                                                \
            RDTSC_START(cycles_start);                                                                                                               \
            if (test != expected)                                                                                                                    \
            {                                                                                                                                        \
                printf("not expected (%d , %d )", (int)test, (int)expected);                                                                         \
                break;                                                                                                                               \
            }                                                                                                                                        \
            RDTSC_STOP(cycles_final);                                                                                                                \
            cycles_diff = (cycles_final - cycles_start - global_rdtsc_overhead);                                                                     \
            if (cycles_diff < min_diff)                                                                                                              \
                min_diff = cycles_diff;                                                                                                              \
            sum_diff += cycles_diff;                                                                                                                 \
        }                                                                                                                                            \
        uint64_t S             = size;                                                                                                               \
        float cycle_per_op     = (min_diff) / (double)S;                                                                                             \
        float avg_cycle_per_op = (sum_diff) / ((double)S * repeat);                                                                                  \
        if (verbose)                                                                                                                                 \
            printf(" %.3f %s per operation (best) ", cycle_per_op, unitname);                                                                        \
        if (verbose)                                                                                                                                 \
            printf("\t%.3f %s per operation (avg) ", avg_cycle_per_op, unitname);                                                                    \
        if (verbose)                                                                                                                                 \
            printf("\n");                                                                                                                            \
        if (!verbose)                                                                                                                                \
            printf(" %.3f ", cycle_per_op);                                                                                                          \
        fflush(NULL);                                                                                                                                \
    } while (0)

// like BEST_TIME, but no check
#define BEST_TIME_NOCHECK(test, pre, repeat, size, verbose)                                                                                          \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (global_rdtsc_overhead == UINT64_MAX)                                                                                                     \
        {                                                                                                                                            \
            RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);                                                                                      \
        }                                                                                                                                            \
        if (verbose)                                                                                                                                 \
            printf("%-60s\t: ", #test);                                                                                                              \
        fflush(NULL);                                                                                                                                \
        uint64_t cycles_start, cycles_final, cycles_diff;                                                                                            \
        uint64_t min_diff = (uint64_t)-1;                                                                                                            \
        uint64_t sum_diff = 0;                                                                                                                       \
        for (int i = 0; i < repeat; i++)                                                                                                             \
        {                                                                                                                                            \
            pre;                                                                                                                                     \
            __asm volatile("" ::: /* pretend to clobber */ "memory");                                                                                \
            RDTSC_START(cycles_start);                                                                                                               \
            test;                                                                                                                                    \
            RDTSC_STOP(cycles_final);                                                                                                                \
            cycles_diff = (cycles_final - cycles_start - global_rdtsc_overhead);                                                                     \
            if (cycles_diff < min_diff)                                                                                                              \
                min_diff = cycles_diff;                                                                                                              \
            sum_diff += cycles_diff;                                                                                                                 \
        }                                                                                                                                            \
        uint64_t S             = size;                                                                                                               \
        float cycle_per_op     = (min_diff) / (double)S;                                                                                             \
        float avg_cycle_per_op = (sum_diff) / ((double)S * repeat);                                                                                  \
        if (verbose)                                                                                                                                 \
            printf(" %.3f %s per operation (best) ", cycle_per_op, unitname);                                                                        \
        if (verbose)                                                                                                                                 \
            printf("\t%.3f %s per operation (avg) ", avg_cycle_per_op, unitname);                                                                    \
        if (verbose)                                                                                                                                 \
            printf("\n");                                                                                                                            \
        if (!verbose)                                                                                                                                \
            printf(" %.3f ", cycle_per_op);                                                                                                          \
        fflush(NULL);                                                                                                                                \
    } while (0)

// like BEST_TIME except that we run a function to check the result
#define BEST_TIME_CHECK(test, check, pre, repeat, size, verbose)                                                                                     \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (global_rdtsc_overhead == UINT64_MAX)                                                                                                     \
        {                                                                                                                                            \
            RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);                                                                                      \
        }                                                                                                                                            \
        if (verbose)                                                                                                                                 \
            printf("%-60s\t: ", #test);                                                                                                              \
        fflush(NULL);                                                                                                                                \
        uint64_t cycles_start, cycles_final, cycles_diff;                                                                                            \
        uint64_t min_diff = (uint64_t)-1;                                                                                                            \
        uint64_t sum_diff = 0;                                                                                                                       \
        for (int i = 0; i < repeat; i++)                                                                                                             \
        {                                                                                                                                            \
            pre;                                                                                                                                     \
            __asm volatile("" ::: /* pretend to clobber */ "memory");                                                                                \
            RDTSC_START(cycles_start);                                                                                                               \
            test;                                                                                                                                    \
            RDTSC_STOP(cycles_final);                                                                                                                \
            if (!check)                                                                                                                              \
            {                                                                                                                                        \
                printf("error");                                                                                                                     \
                break;                                                                                                                               \
            }                                                                                                                                        \
            cycles_diff = (cycles_final - cycles_start - global_rdtsc_overhead);                                                                     \
            if (cycles_diff < min_diff)                                                                                                              \
                min_diff = cycles_diff;                                                                                                              \
            sum_diff += cycles_diff;                                                                                                                 \
        }                                                                                                                                            \
        uint64_t S             = size;                                                                                                               \
        float cycle_per_op     = (min_diff) / (double)S;                                                                                             \
        float avg_cycle_per_op = (sum_diff) / ((double)S * repeat);                                                                                  \
        if (verbose)                                                                                                                                 \
            printf(" %.3f cycles per operation (best) ", cycle_per_op);                                                                              \
        if (verbose)                                                                                                                                 \
            printf("\t%.3f cycles per operation (avg) ", avg_cycle_per_op);                                                                          \
        if (verbose)                                                                                                                                 \
            printf("\n");                                                                                                                            \
        if (!verbose)                                                                                                                                \
            printf(" %.3f ", cycle_per_op);                                                                                                          \
        fflush(NULL);                                                                                                                                \
    } while (0)

#endif