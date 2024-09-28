// Copyright 2020 The Division Authors.
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

#include "plf_list.h"
#include "tsl/ordered_map.h"
#include <benchmark/benchmark.h>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <deque>
#include <iostream>
#include <list>
#include <unordered_map>

using mode               = boost::intrusive::link_mode<boost::intrusive::auto_unlink>;
using constant_time_size = boost::intrusive::constant_time_size<false>;

struct SList : public boost::intrusive::list_base_hook<mode>
{
    int x;
};

template <typename V> void Splice(V &&v1, V &&v2)
{
    if (v1.empty())
    {
        v1.splice(v1.end(), v2);
        return;
    }
    v2.splice(v2.end(), v1);
}
static void BenchListSplice(benchmark::State &state)
{
    std::list<SList> v;
    std::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(v, v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSplice)->Range(1, 65536);

static void BenchListSwap(benchmark::State &state)
{
    int r = 0;
    std::list<SList> v;
    std::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    for (auto _ : state)
    {
        v.swap(v1);
        r = v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchListSwap)->Range(1, 65536);

static void BenchDequeSwap(benchmark::State &state)
{
    std::deque<SList> v;
    std::deque<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        v.swap(v1);
        r = v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchDequeSwap)->Range(1, 65536);

static void BenchIntrusiveListSplice(benchmark::State &state)
{
    boost::intrusive::list<SList, constant_time_size> vt;
    boost::intrusive::list<SList, constant_time_size> v;
    std::vector<SList> lst(state.range(0));
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(lst[i]);
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(vt, v);
        r = v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSplice)->Range(1, 65536);

static void BenchIntrusiveListSwap(benchmark::State &state)
{
    boost::intrusive::list<SList, constant_time_size> vt;
    boost::intrusive::list<SList, constant_time_size> v;
    std::vector<SList> lst{static_cast<size_t>(state.range(0))};
    for (auto i = 0; i < state.range(0); i++)
    {
        v.push_back(lst[i]);
    }
    int r = 0;
    for (auto _ : state)
    {
        vt.swap(v);
        r = v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchIntrusiveListSwap)->Range(1, 65536);

static void BenchPlfListSplice(benchmark::State &state)
{
    plf::list<SList> v;
    plf::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        Splice(v,v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListSplice)->Range(1, 65536);

static void BenchPlfListSwap(benchmark::State &state)
{
    plf::list<SList> v;
    plf::list<SList> v1;
    for (auto i = 0; i < state.range(0); i++)
    {
        v1.push_back({});
    }
    int r = 0;
    for (auto _ : state)
    {
        v.swap(v1);
        r += v.size();
    }
    benchmark::DoNotOptimize(r);
}

BENCHMARK(BenchPlfListSwap)->Range(1, 65536);

struct WFGUID
{
    int64_t nData64;
    int64_t nHead64;
    inline bool operator==(const WFGUID& id) const
    {
        return (this->nData64 == id.nData64) && (this->nHead64 == id.nHead64);
    }

    inline bool operator!=(const WFGUID& id) const
    {
        return (this->nData64 != id.nData64) || (this->nHead64 != id.nHead64);
    }

};

namespace std
{
    template <> class hash<WFGUID>
    {
      public:
        inline size_t operator()(const WFGUID &xGUID) const
        {
            return /*hash<WFINT64>{}(xGUID.nHead64) * 31 + */ hash<int64_t>{}(xGUID.nData64);
        }
    };
}
using KeyT = std::pair<WFGUID , std::string>;


struct WFPairHash
{
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        return std::hash<T1>()(p.first) * 31 + std::hash<T2>()(p.second);
    }
};

struct ListData : public boost::intrusive::list_base_hook<mode>
{
    ListData(const std::pair<WFGUID , std::string>& _xKey, const uint64_t _nTimeOut, const uint64_t _nTimeOutCount)
        : xKey(_xKey),
          nTimeOut(_nTimeOut),
          nTimeOutCount(_nTimeOutCount)
    {
    }

    ListData() = default;

    std::pair<WFGUID , std::string> xKey;                      // key
    uint64_t nTimeOut;                 // 到期时间
    uint64_t nTimeOutCount; // 到期次数
};

int main(int argc, char **argv)
{
    using MapT = tsl::vector_map<KeyT, std::unique_ptr<ListData>, WFPairHash>;
    boost::intrusive::list<ListData, constant_time_size> vt;
    MapT mapData;
    for(auto i=0;i<10;i++)
    {
        KeyT key{WFGUID{i,i},""};
        mapData[key]=std::make_unique<ListData>();
        vt.push_back( *mapData[key]);
        vt.back().nTimeOut=i;
    }

    for(auto &&it:vt)
    {
        std::cout<<it.nTimeOut<<'\n';
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}