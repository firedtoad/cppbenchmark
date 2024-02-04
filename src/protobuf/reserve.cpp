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

#include "algorithm/container.h"
#include "report.pb.h"
#include "tsl/array-hash/array_map.h"
#include "tsl/ordered_map.h"
#include "tsl/ordered_set.h"
#include "utils/rss.h"
#include <benchmark/benchmark.h>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <experimental/unordered_map>
#include <parallel_hashmap/phmap.h>
#include <vector>

static void BenchPHMAP(benchmark::State &state)
{
    srand(time(nullptr));
    static phmap::parallel_flat_hash_map_m<int, int> mp;
    for (auto _ : state)
    {
        //           auto r=mp.try_emplace_l(1,[&](auto & it){
        //                        it.second=10;
        //                    });
        auto r = mp.try_emplace( random(), 1);

        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BenchPHMAP)->ThreadRange(1, 4);

static void BenchAdd(benchmark::State &state)
{
    for (auto _ : state)
    {
        pb_report::Ground report;
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}
// BENCHMARK(BenchAdd)->Range(1, 65536);

static void BenchAddArena(benchmark::State &state)
{
    for (auto _ : state)
    {
        google::protobuf::Arena arena;
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}
// BENCHMARK(BenchAddArena)->Range(1, 65536);

static void BenchAddArenaOptions(benchmark::State &state)
{
    for (auto _ : state)
    {
        google::protobuf::ArenaOptions options;
        options.start_block_size = 4096;
        options.max_block_size   = 1024 * 1024;
        google::protobuf::Arena arena(options);
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}
// BENCHMARK(BenchAddArenaOptions)->Range(1, 65536);

static void BenchReserve(benchmark::State &state)
{

    for (auto _ : state)
    {
        pb_report::Ground report;
        report.mutable_attackerform()->Reserve(state.range(0));
        report.mutable_defenderform()->Reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}

// BENCHMARK(BenchReserve)->Range(1, 65536);

static void BenchReserveArena(benchmark::State &state)
{
    for (auto _ : state)
    {
        google::protobuf::Arena arena;
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        report.mutable_attackerform()->Reserve(state.range(0));
        report.mutable_defenderform()->Reserve(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            report.add_attackerform()->add_units();
            report.add_defenderform()->add_units();
        }
        benchmark::DoNotOptimize(report);
    }
}

// BENCHMARK(BenchReserveArena)->Range(1, 65536);

struct StrPtrHash
{
    size_t operator()(const std::string *key) const
    {
        return std::hash<std::string>{}(*key);
    }
};

struct StrPtrEqual
{
    bool operator()(const std::string *self, const std::string *other) const
    {
        if (self != nullptr && other != nullptr)
        {
            return *self == *other;
        }
        return false;
    }
};

const int SIZE = 65536;
int main(int argc, char **argv)
{

    rusage rUsage;
    FillRSS(rUsage);
    pb_report::Ground report;
    for (auto i = 0; i < SIZE; i++)
    {
        report.add_attackerform()->add_units();
        report.add_defenderform()->add_units();
    }
    benchmark::DoNotOptimize(report);
    PrintUsage(rUsage);
    FillRSS(rUsage);

    pb_report::Ground report1;
    report1.mutable_attackerform()->Reserve(SIZE);
    report1.mutable_defenderform()->Reserve(SIZE);
    for (auto i = 0; i < SIZE; i++)
    {
        report1.add_attackerform()->add_units();
        report1.add_defenderform()->add_units();
    }
    benchmark::DoNotOptimize(report);
    PrintUsage(rUsage);
    {
        FillRSS(rUsage);
        char buff[1024 * 1024 * 4];
        google::protobuf::ArenaOptions options;
        options.start_block_size = 4096;
        options.max_block_size   = 1024 * 1024;
        google::protobuf::Arena arena(options);
        pb_report::Ground &report = *google::protobuf::Arena::CreateMessage<pb_report::Ground>(&arena);
        report.mutable_attackerform()->Reserve(SIZE);
        report.mutable_defenderform()->Reserve(SIZE);
        //        for (auto i = 0; i < SIZE; i++)
        //        {
        //            report.add_attackerform()->add_units();
        //            report.add_defenderform()->add_units();
        //        }
        std::cout << arena.SpaceUsed() << ":" << arena.SpaceAllocated() << '\n';
        PrintUsage(rUsage);
    }
    std::unordered_map<int, int> mp = {
        {1, 2},
        {2, 2},
        {3, 2},
    };
    std::experimental::erase_if(mp, [](auto &&it) { return true; });

    {
        FillRSS(rUsage);
        google::protobuf::Arena arena;
        google::protobuf::RepeatedPtrField<int> data(&arena);
        data.Reserve(1000000);
        for (auto i = 0; i < 1000000; i++)
        {
            *data.Add() = i;
        }
        PrintUsage(rUsage);
    }

    {
        FillRSS(rUsage);
        google::protobuf::Arena arena;
        google::protobuf::RepeatedField<int> data(&arena);
        data.Reserve(1000000);
        for (auto i = 0; i < 1000000; i++)
        {
            data.Add(i);
        }
        PrintUsage(rUsage);
    }

    {
        FillRSS(rUsage);
        boost::object_pool<std::string> pool;
        for (auto i = 0; i < 150000; i++)
        {
            auto sz = (i % 32) + 16;
            //            auto p  = pool.allocate();
            auto p = pool.construct();
            p->resize(sz);
            //            auto p = new char[sz]();
            //            memset(p, 0, sz);
            //            auto p = new std::string(sz, '1');
            DoNotOptimize(p);
        }
        PrintUsage(rUsage);
    }

    //    {
    //        WFMsg::ObjectRecordInt xMsg;
    //        for (auto i = 0; i < 10000; i++)
    //        {
    //            auto &xAdd = *xMsg.add_record_list();
    //            xAdd.set_row(i);
    //            xAdd.set_col(i);
    //            xAdd.set_data(i);
    //        }
    //        std::cout << xMsg.SerializeAsString().size() << '\n';
    //    }
    //
    //    {
    //        WFMsg::ObjectRecordIntPacked xMsg;
    //        auto &xList = *xMsg.mutable_record_list();
    //        for (auto i = 0; i < 10000; i++)
    //        {
    //
    //            xList.add_row(i);
    //            xList.add_col(i);
    //            xList.add_data(i);
    //        }
    //        std::cout << xMsg.SerializeAsString().size() << '\n';
    //    }
    std::set<std::shared_ptr<pb_report::Ground>> sint;

    for (auto i = 0; i < 10; i++)
    {
        auto p = std::make_shared<pb_report::Ground>();
        p->mutable_attackerform()->Reserve(i);
        sint.insert(p);
    }

    for (auto it : sint)
    {
        std::cout << it->mutable_attackerform()->Capacity() << '\n';
    }

    tsl::vector_set<int> vs;
    vs.insert(7);
    vs.insert(4);
    vs.insert(2);
    vs.insert(5);
    for (auto it : vs)
    {
        std::cout << it << '\n';
    }
    tsl::vector_map<int, std::weak_ptr<int>> set_weak;
    {
        auto p      = std::make_shared<int>();
        set_weak[1] = p;
        tsl::vector_map<int, std::shared_ptr<int>> set_shared;
        set_shared[1] = p;
    }

    auto it = set_weak.find(1);
    if (it != set_weak.end())
    {
        auto p = it->second.lock();
        if (p)
        {
            std::cout << p << '\n';
        }
    }

    std::deque<int> d1;
    d1.emplace_back(10);
    d1.emplace_back(20);
    std::deque<int> d2;
    d2.emplace_back(100);
    d2.emplace_back(200);

    {
        auto d3 = d1;
        auto d4 = d2;
        d3.insert(d3.begin(), d4.begin(), d4.end());
        d4.clear();
        c_copy(d3, std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
        c_copy(d4, std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
    }

    {
        auto d3 = d1;
        auto d4 = d2;
        c_move(d4, std::front_inserter(d3));
        c_copy(d3, std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
        c_copy(d4, std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
    }

    {
        auto d3 = d1;
        auto d4 = d2;
        c_move(d4, std::back_inserter(d3));

        c_copy(d3, std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
        c_copy(d4, std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
    }
    {
        struct Test
        {
            ~Test()
            {
                std::cout << __PRETTY_FUNCTION__ << '\n';
            }
        };
        google::protobuf::Arena xArena;


        //        delete pTest;
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}