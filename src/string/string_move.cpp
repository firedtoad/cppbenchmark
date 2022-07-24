#include <benchmark/benchmark.h>
struct LogCopy
{
    template <typename F>
    LogCopy(const std::string &file, int line, const std::string &fun, F &&log)
        : m_file(file), m_line(line), m_fun(fun), m_log(std::forward<F>(log)())
    {
    }
    std::string m_file;
    int m_line;
    std::string m_fun;
    std::string m_log;
};

struct LogMove
{
    template <typename F>
    LogMove(std::string file, int line, std::string fun, F &&log)
        : m_file(std::move(file)), m_line(line), m_fun(std::move(fun)), m_log(std::forward<F>(log)())
    {
    }
    std::string m_file;
    int m_line;
    std::string m_fun;
    std::string m_log;
};

template <typename S> static void BenchStringMove(benchmark::State &state)
{
    auto fn = [] { return "asdasdasddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"; };
    for (auto _ : state)
    {
        S s(__FILE__, __LINE__, __PRETTY_FUNCTION__, std::move(fn));
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_TEMPLATE(BenchStringMove, LogCopy);
BENCHMARK_TEMPLATE(BenchStringMove, LogMove);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
