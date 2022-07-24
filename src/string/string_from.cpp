
#include <absl/strings/numbers.h>
#include <absl/strings/str_format.h>
#include <benchmark/benchmark.h>
#include <boost/lexical_cast.hpp>
#include <charconv>
#include <cmath>
#include <google/protobuf/stubs/strutil.h>
#include <random>
#include <sstream>

uint64_t tag = 1234567890123456;
float tagf   = M_PI;

template <typename S, typename T> void NumberToString(T &t, std::string &str)
{
    thread_local static S oss;
    oss.str("");
    oss << t;
    str = oss.str();
}

template <typename S, typename T> static void BenchStreamStringFrom(benchmark::State &state)
{
    std::string r;
    for (auto _ : state)
    {
        NumberToString<S, T>(tag, r);
        benchmark::DoNotOptimize(r);
    }
}

template <typename S, typename T> static void BenchStreamStringFromF(benchmark::State &state)
{
    std::string r;
    for (auto _ : state)
    {
        NumberToString<S, T>(tagf, r);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStreamStringFrom, std::stringstream, uint64_t);
BENCHMARK_TEMPLATE(BenchStreamStringFrom, std::ostringstream, uint64_t);

BENCHMARK_TEMPLATE(BenchStreamStringFromF, std::stringstream, float);
BENCHMARK_TEMPLATE(BenchStreamStringFromF, std::ostringstream, float);

template <typename T> static void BenchItoa(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buf[64]{};
        auto r = itoa(tag, buf, 10);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchItoa, uint64_t);

template <typename T> static void BenchGcvt(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buf[64]{};
        auto r = gcvt(tagf, 11, buf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchGcvt, float);

template <typename T> static void BenchToString(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::to_string(tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchToString, uint64_t);

template <typename T> static void BenchToStringF(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = std::to_string(tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchToStringF, float);

template <typename T> static void BenchStdToChars(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buf[64]{};
        auto r = std::to_chars(buf, buf + 64, tag, 10);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStdToChars, uint64_t);

template <typename T> static void BenchStdToCharsF(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buf[64]{};
        auto r = std::to_chars(buf, buf + 64, tagf);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStdToCharsF, float);

template <typename T> static void BenchBoostStringFrom(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(tag);
        benchmark::DoNotOptimize(r);
    }
}
template <typename T> static void BenchBoostStringTof(benchmark::State &state)
{
    for (auto _ : state)
    {
        T r = boost::lexical_cast<T>(tagf);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostStringFrom, std::string);

BENCHMARK_TEMPLATE(BenchBoostStringTof, std::string);

template <typename T> static void BenchAbFastIntToBuffer(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        char buf[64]{};
        if (absl::numbers_internal::FastIntToBuffer(tag, buf))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbFastIntToBuffer, uint64_t);

template <typename T> static void BenchAbSixDigitsToBuffer(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        char buf[64]{};
        if (absl::numbers_internal::SixDigitsToBuffer(tagf, buf))
        {
        }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbSixDigitsToBuffer, float);

template <typename T> static void BenchAbStrFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = absl::StrFormat("%d", tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbStrFormat, uint64_t);

template <typename T> static void BenchAbStrFormatF(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = absl::StrFormat("%f", tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchAbStrFormatF, float);

template <typename T> static void BenchPBSimpleItoa(benchmark::State &state)
{
    T r = 0;
    for (auto _ : state)
    {
        char buf[64]{};
        auto r = google::protobuf::FastUInt64ToBuffer(tag, buf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchPBSimpleItoa, uint64_t);

int main(int argc, char **argv)
{

    tag  = std::mt19937_64{std::random_device{}()}();
    tagf = std::mt19937_64{std::random_device{}()}();
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}