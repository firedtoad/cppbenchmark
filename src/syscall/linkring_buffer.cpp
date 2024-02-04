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
#include <deque>
#include <vector>

struct RingBuffer
{
    std::vector<char> m_data;
    RingBuffer() {}

    int Append(const char *data, int len)
    {
        m_data.insert(m_data.end(), &data[0], &data[len]);
        return m_data.size();
    }
};

struct LinkBuffer
{
    struct IOBuf
    {
        void *data;
        int len;
    };

    std::deque<IOBuf> m_deque;
    LinkBuffer() {}
    ~LinkBuffer()
    {
        for (auto &it : m_deque)
        {
            if (it.data)
            {
                free(it.data);
                it.data = nullptr;
                it.len  = 0;
            }
        }
    }

    int Append(char *data, int len)
    {
        IOBuf buf{};
        buf.data = data;
        buf.len  = len;
        m_deque.push_back(buf);
        return m_deque.size();
    }
};

template <size_t N> static void BenchRingBuffer(benchmark::State &state)
{
    std::vector<char> data(state.range(0), '0');
    for (auto _ : state)
    {
        RingBuffer buffer;
        for (size_t i{}; i < N; i++)
        {
            auto r = buffer.Append(data.data(), data.size());
            benchmark::DoNotOptimize(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchRingBuffer, 128)->Range(1,1024);

template <size_t N> static void BenchLinkBuffer(benchmark::State &state)
{
    for (auto _ : state)
    {
        LinkBuffer buffer;
        for (size_t i{}; i < N; i++)
        {
            auto pData = (char *)malloc(state.range(0));
            auto r     = buffer.Append(pData, state.range(0));
            benchmark::DoNotOptimize(r);
        }
    }
}

BENCHMARK_TEMPLATE(BenchLinkBuffer, 128)->Range(1,1024);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
