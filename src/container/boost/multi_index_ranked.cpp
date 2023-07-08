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
#include <boost/container/flat_map.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ranked_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <iostream>

struct TagRank
{
};

template <typename V>
using MultiHashContainer = boost::multi_index::multi_index_container<
    V, boost::multi_index::indexed_by<boost::multi_index::hashed_unique<boost::multi_index::identity<V>>,
                                      boost::multi_index::ranked_non_unique<boost::multi_index::tag<TagRank>, boost::multi_index::identity<V>>>>;

int main(int argc, char **argv)
{

    MultiHashContainer<int> ranked;
    for (auto i = 1; i < 2; i++)
    {
        ranked.insert(i);
    }
    ranked.equal_range(10);
    auto p = ranked.get<TagRank>().equal_range_rank(0);
    std::cout << p.first << ' ' << p.second << '\n';
    auto begin = ranked.get<TagRank>().nth(0);
    auto end   = ranked.get<TagRank>().nth(0);
    do
    {
        std::cout << *begin << '\n';
    } while (begin-- != end);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}