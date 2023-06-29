// Copyright 2023 The Division Authors.
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

#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

template <size_t wait> static void BenchEpollWait(benchmark::State &state)
{
    int efd = epoll_create1(EPOLL_CLOEXEC);
    epoll_event events[1024];
    int fd = open("/proc/stat", 0);
    epoll_event evt;
    evt.events = EPOLLIN;
    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &evt);
    for (auto _ : state)
    {
        int r = epoll_wait(efd, events, 1024, wait);
        benchmark::DoNotOptimize(r);
    }
    close(fd);
    close(efd);
}
BENCHMARK_TEMPLATE(BenchEpollWait, 1);
BENCHMARK_TEMPLATE(BenchEpollWait, 0);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
