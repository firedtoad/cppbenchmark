//
// Created by zhangwenhao.101 on 2022/7/19.
//

#include <benchmark/benchmark.h>
#include <vector>

struct Pod {
  uint64_t i;
  Pod() noexcept = default;
  explicit Pod(uint64_t i_) noexcept : i(i_) {}
  Pod(const Pod &pod) noexcept = default;
  Pod(Pod &&pod) noexcept = default;
  Pod &operator=(const Pod &pod) noexcept = default;
  Pod &operator=(Pod &&pod) noexcept = default;
  bool operator<(const Pod &p) const { return i < p.i; }
};

struct NonPod {
  uint64_t i = 0;
  NonPod() noexcept = default;
  explicit NonPod(uint64_t i_) noexcept : i(i_) {}
  NonPod(const NonPod &p) noexcept = default;
  NonPod(NonPod &&p) noexcept = default;
  NonPod &operator=(const NonPod &p) noexcept = default;
  NonPod &operator=(NonPod &&p) noexcept = default;
  bool operator<(const NonPod &p) const { return i < p.i; }
};

template <typename V, typename P>
static void BenchInsertPod(benchmark::State &state) {

  for (auto _ : state) {
    V v;
    for (auto i = 0; i < state.range(0); i++) {
      v.push_back({});
    }
  }
}

 BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<NonPod>, NonPod)
     ->Range(2, 65536)
     ->MeasureProcessCPUTime();
 BENCHMARK_TEMPLATE(BenchInsertPod, std::vector<Pod>, Pod)
     ->Range(2, 65536)
     ->MeasureProcessCPUTime();

template <typename P>
static void BenchUninitializedCopy(benchmark::State &state) {
  std::vector<P> pod(state.range(0));
  std::vector<P> dest(state.range(0));
  for (auto _ : state) {
    std::uninitialized_copy(pod.begin(), pod.end(), dest.begin());
    benchmark::DoNotOptimize(dest);
  }
}

BENCHMARK_TEMPLATE(BenchUninitializedCopy, NonPod)
    ->Range(2, 65536)
    ->MeasureProcessCPUTime();
BENCHMARK_TEMPLATE(BenchUninitializedCopy, Pod)
    ->Range(2, 65536)
    ->MeasureProcessCPUTime();

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}