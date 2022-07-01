
#include <benchmark/benchmark.h>
#include <sstream>
#include <random>
#include <fmt/format.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <boost/algorithm/string.hpp>
#include <google/protobuf/stubs/strutil.h>

std::string tag = "1234567890123456";
std::string tag1 = "1234567890123456";

static void BenchStrNCat(benchmark::State &state) {
    for (auto _ : state) {
        char dest[128]{};
        strncat(dest, tag.c_str(), tag.size());
        strncat(&dest[tag.size()], ":", 2);
        strncat(&dest[tag.size() + 1], tag1.c_str(), tag1.size());
        benchmark::DoNotOptimize(dest);
    }
}
BENCHMARK(BenchStrNCat);

static void BenchStdStringCat(benchmark::State &state) {
    for (auto _ : state) {
        auto r = tag;
        r += ":";
        r += tag1;
        r += tag1;
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchStdStringCat);

static void BenchStreamStringCat(benchmark::State &state) {

    for (auto _ : state) {
        std::stringstream ss;
        ss << tag << ":" << tag1 << tag1;
        auto r = ss.str();
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchStreamStringCat);

static void BenchBoostStrCat(benchmark::State &state) {

    for (auto _ : state) {
        auto r = boost::join<std::vector<std::string>>({tag, ":", tag1}, "");
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchBoostStrCat);

static void BenchAbStrCat(benchmark::State &state) {
    for (auto _ : state) {
        auto r = absl::StrCat(tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchAbStrCat);

static void BenchPBStrCat(benchmark::State &state) {
    for (auto _ : state) {
        auto r = google::protobuf::StrCat(tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchPBStrCat);

static void BenchAbStrFormat(benchmark::State &state) {
    for (auto _ : state) {
        auto r = absl::StrFormat("%s%s%s%s", tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchAbStrFormat);

static void BenchFormat(benchmark::State &state) {
    for (auto _ : state) {
        auto r = fmt::format("{}{}{}{}", tag, ":", tag1, tag1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BenchFormat);

int main(int argc, char **argv) {

    tag = std::to_string(std::mt19937_64{std::random_device{}()}());
    tag1 = std::to_string(std::mt19937_64{std::random_device{}()}());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}