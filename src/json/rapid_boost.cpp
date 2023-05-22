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

#define RAPIDJSON_SSE42
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "yyjson.h"
#include <benchmark/benchmark.h>
//#include <boost/json/src.hpp>
#include <iostream>
#include <sstream>

static unsigned long xorshf96()
{ /* A George Marsaglia generator, period 2^96-1 */
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;

    z = t ^ x ^ y;
    return z;
}

static inline unsigned long _random()
{
    return xorshf96();
}

static void BenchYYJson(benchmark::State &state)
{
    std::string res;
    for (auto _ : state)
    {
        yyjson_mut_doc *doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val *root = yyjson_mut_obj(doc);
        for (auto i = 0; i < state.range(0); i++)
        {
            yyjson_mut_obj_add_int(doc, root, std::to_string(i + _random()).c_str(), i);
        }
        const char *json = yyjson_mut_write(doc, 0, NULL);
        if (json)
        {
            free((void *)json);
        }
        yyjson_mut_doc_free(doc);
        benchmark::DoNotOptimize(doc);
        benchmark::DoNotOptimize(json);
    }
}

BENCHMARK(BenchYYJson)->Range(1, 128);

static void BenchRapid(benchmark::State &state)
{
    std::string res;
    for (auto _ : state)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        writer.StartObject();
        for (auto i = 0; i < state.range(0); i++)
        {
            writer.Key(std::to_string(i + _random()).c_str());
            writer.Uint64(i);
        }
        writer.EndObject();
        benchmark::DoNotOptimize(buffer);
    }
}

static void BenchRapidReserve(benchmark::State &state)
{
    std::string res;
    for (auto _ : state)
    {
        rapidjson::StringBuffer buffer(0, state.range(0) * 64);
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        writer.StartObject();
        for (auto i = 0; i < state.range(0); i++)
        {
            writer.Key(std::to_string(i + _random()).c_str());
            writer.Uint64(i);
        }
        writer.EndObject();
        benchmark::DoNotOptimize(buffer);
    }
}

static void BenchRapidDocument(benchmark::State &state)
{
    std::string res;
    for (auto _ : state)
    {
        rapidjson::Document doc;
        doc.SetObject();
        doc.MemberReserve(state.range(0), doc.GetAllocator());
        std::vector<std::string> k_vec;
        k_vec.resize(state.range(0));
        for (auto i = 0; i < state.range(0); i++)
        {
            k_vec[i] = std::to_string(_random());
            doc.AddMember(rapidjson::Value(k_vec[i].c_str(), k_vec[i].size()), rapidjson::Value(i), doc.GetAllocator());
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        benchmark::DoNotOptimize(buffer);
    }
}

BENCHMARK(BenchRapid)->Range(1, 128);
BENCHMARK(BenchRapidReserve)->Range(1, 128);
BENCHMARK(BenchRapidDocument)->Range(1, 128);

//static void BenchBoostJson(benchmark::State &state)
//{
//    std::ostringstream oss;
//    for (auto _ : state)
//    {
//        boost::json::object object(boost::json::make_shared_resource<boost::json::monotonic_resource>());
//        for (auto i = 0; i < state.range(0); i++)
//        {
//            object[std::to_string(_random())] = i;
//        }
//        oss << object;
//        oss.str("");
//    }
//}

//BENCHMARK(BenchBoostJson)->Range(1, 128);
int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}