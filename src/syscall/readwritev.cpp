// Copyright 2024 The Division Authors.
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

#include <array>
#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <vector>

template <size_t N> static void BenchReadWrite(benchmark::State &state)
{
    int fd[2];
    socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0, fd);
    char rBuff[256];
    char wBuff[256];
    for (auto _ : state)
    {
        for (size_t i = 0; i < N; i++)
        {
            int w = write(fd[0], wBuff, 256);
            int r = read(fd[1], rBuff, 256);
            benchmark::DoNotOptimize(w);
            benchmark::DoNotOptimize(r);
        }
    }
    close(fd[0]);
    close(fd[1]);
}

BENCHMARK_TEMPLATE(BenchReadWrite, 128);
BENCHMARK_TEMPLATE(BenchReadWrite, 256);

template <size_t N> static void BenchReadWriteV(benchmark::State &state)
{
    int fd[2];
    socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0, fd);
    struct iovec rVec[N];
    struct iovec wVec[N];
    std::vector<std::array<char, 256>> rVecBuff(N);
    std::vector<std::array<char, 256>> wVecBuff(N);
    for (size_t i = 0; i < N; i++)
    {
        rVec[i].iov_base = rVecBuff[i].data();
        rVec[i].iov_len  = rVecBuff[i].size();
        wVec[i].iov_base = wVecBuff[i].data();
        wVec[i].iov_len  = wVecBuff[i].size();
    }

    for (auto _ : state)
    {
        int w = writev(fd[0], wVec, N);
        int r = readv(fd[1], rVec, N);
        benchmark::DoNotOptimize(w);
        benchmark::DoNotOptimize(r);

    }
    close(fd[0]);
    close(fd[1]);
}

BENCHMARK_TEMPLATE(BenchReadWriteV, 128);
BENCHMARK_TEMPLATE(BenchReadWriteV, 256);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
