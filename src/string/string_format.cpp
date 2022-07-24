
#include <absl/strings/str_format.h>
#include <benchmark/benchmark.h>
#include <boost/format.hpp>
#include <fmt/format.h>
#include <random>
#include <sstream>

uint64_t tag = 1234567890123456;
float tagf   = M_PI;

template <typename S, typename T> void StringFormat(T &t, std::string &str)
{
    S oss(str);
    oss << t;
    str = oss.str();
}

template <typename S, typename T> static void BenchStreamStringFormat(benchmark::State &state)
{
    std::string r;
    for (auto _ : state)
    {
        StringFormat<S, T>(tag, r);
        benchmark::DoNotOptimize(r);
    }
}

template <typename S, typename T> static void BenchStreamStringFormatF(benchmark::State &state)
{
    std::string r;
    for (auto _ : state)
    {
        StringFormat<S, T>(tagf, r);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchStreamStringFormat, std::stringstream, uint64_t);
BENCHMARK_TEMPLATE(BenchStreamStringFormat, std::ostringstream, uint64_t);

BENCHMARK_TEMPLATE(BenchStreamStringFormatF, std::stringstream, float);
BENCHMARK_TEMPLATE(BenchStreamStringFormatF, std::ostringstream, float);

template <typename T> static void BenchSnprintf(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buff[256]{};
        auto r = snprintf(buff, 256, "%llu", tag);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchSnprintf, uint64_t);

template <typename T> static void BenchSnprintfFloat(benchmark::State &state)
{
    for (auto _ : state)
    {
        char buff[256]{};
        auto r = snprintf(buff, 256, "%f", tagf);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchSnprintfFloat, float);

template <typename T> static void BenchBoostStringFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        ;
        auto r = (boost::format{"%d"} % tag).str();
        benchmark::DoNotOptimize(r);
    }
}

template <typename T> static void BenchBoostStringFormatf(benchmark::State &state)
{
    for (auto _ : state)
    {
        ;
        auto r = (boost::format{"%f"} % tag).str();
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_TEMPLATE(BenchBoostStringFormat, std::string);
BENCHMARK_TEMPLATE(BenchBoostStringFormatf, std::string);

template <typename T> static void BenchFmtStrFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = fmt::format("{}", tag);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchFmtStrFormat, uint64_t);

template <typename T> static void BenchFmtStrFormatf(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto r = fmt::format("{}", tagf);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK_TEMPLATE(BenchFmtStrFormatf, float);

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

int main(int argc, char **argv)
{

    tag  = std::mt19937_64{std::random_device{}()}();
    tagf = std::mt19937_64{std::random_device{}()}();
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}