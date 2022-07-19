#include <benchmark/benchmark.h>
#include <functional>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> combined_logger;
#define EXLOG_INFO(...)                                                        \
  combined_logger->log(                                                        \
      spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},                 \
      spdlog::level::info, __VA_ARGS__)
static void BM_SpdLog(benchmark::State &state) {
  auto i = 0, j = 0, k = 0;
  std::string ss =
      "adssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";
  for (auto _ : state) {
    //    EXLOG_INFO("simple message {} {} {} {}", i++, j++, k++, ss);
    SPDLOG_INFO("simple message {} {} {} {}", i++, j++, k++, ss);
  }
}

BENCHMARK(BM_SpdLog);

int main(int argc, char **argv) {
  auto async_logger =
      spdlog::create_async_nb<spdlog::sinks::rotating_file_sink_mt>(
          "async_file_logger", "async_log.txt", 1024 * 1024 * 128, 10);
  spdlog::set_default_logger(async_logger);
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}