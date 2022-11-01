//
// Created by Administrator on 2022/01/15.
//
#include "flat_hash_map.hpp"
#include "unordered_map.hpp"
#include "parallel_hashmap/phmap.h"
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/container/node_hash_map.h>
#include <absl/container/node_hash_set.h>
#include <benchmark/benchmark.h>
#include <cxxabi.h>
#include <unordered_map>
#include <unordered_set>

std::string demangle(const char *name)
{
    int status                       = -4;
    char *res                        = abi::__cxa_demangle(name, NULL, NULL, &status);
    const char *const demangled_name = (status == 0) ? res : name;
    std::string ret_val(demangled_name);
    free(res);
    return ret_val;
}
template <typename... T> void PrintNode(T &&...t)
{
    (..., (std::cout << demangle(typeid(typename T::node_type).name()) << " size " << sizeof(typename T::node_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintContainer(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(typename T::value_type) << '\n'));
    std::cout << '\n';
}

template <typename... T> void PrintList(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(t) << '\n'));
    std::cout << '\n';
}
struct Hasher
{
    size_t operator()(int key) const
    {
        return 1;
    }
};

template <class M> static void BM_unmap(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (int i = 0; i < 10000; i++)
        {
            m[i] = i;
        }
    }
}
template <class M> static void BM_decay_map(benchmark::State &state)
{
    for (auto _ : state)
    {
        M m;
        for (int i = 0; i < 10000; i++)
        {
            m[i] = i;
        }
    }
}

BENCHMARK_TEMPLATE(BM_unmap, std::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, std::unordered_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, ska::unordered_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, ska::unordered_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, ska::flat_hash_map<int, int>);
// BENCHMARK_TEMPLATE(BM_decay_map, ska::flat_hash_map<int, int,Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, phmap::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, phmap::flat_hash_map<int, int, Hasher>);

BENCHMARK_TEMPLATE(BM_unmap, absl::flat_hash_map<int, int>);
BENCHMARK_TEMPLATE(BM_decay_map, absl::flat_hash_map<int, int, Hasher>);

int main(int argc, char **argv)
{

    PrintNode(phmap::flat_hash_set<uint8_t>{}, phmap::flat_hash_set<uint16_t>{}, phmap::flat_hash_set<uint32_t>{}, phmap::flat_hash_set<uint64_t>{});
    PrintNode(absl::flat_hash_set<uint8_t>{}, absl::flat_hash_set<uint16_t>{}, absl::flat_hash_set<uint32_t>{}, absl::flat_hash_set<uint64_t>{});
    PrintNode(absl::node_hash_set<uint8_t>{}, absl::node_hash_set<uint16_t>{}, absl::node_hash_set<uint32_t>{}, absl::node_hash_set<uint64_t>{});

    PrintContainer(phmap::flat_hash_map<uint8_t, uint8_t>{}, phmap::flat_hash_map<uint16_t, uint16_t>{}, phmap::flat_hash_map<uint32_t, uint32_t>{},
                   phmap::flat_hash_map<uint64_t, uint64_t>{});
    PrintContainer(absl::flat_hash_map<uint8_t, uint8_t>{}, absl::flat_hash_map<uint16_t, uint16_t>{}, absl::flat_hash_map<uint32_t, uint32_t>{},
                   absl::flat_hash_map<uint64_t, uint64_t>{});
    PrintContainer(absl::node_hash_map<uint8_t, uint8_t>{}, absl::node_hash_map<uint16_t, uint16_t>{}, absl::node_hash_map<uint32_t, uint32_t>{},
                   absl::node_hash_map<uint64_t, uint64_t>{});

    PrintContainer(ska::flat_hash_set<uint8_t>{}, ska::flat_hash_set<uint16_t>{}, ska::flat_hash_set<uint32_t>{}, ska::flat_hash_set<uint64_t>{});
    PrintContainer(ska::flat_hash_map<uint8_t, uint8_t>{}, ska::flat_hash_map<uint16_t, uint16_t>{}, ska::flat_hash_map<uint32_t, uint32_t>{},
                   ska::flat_hash_map<uint64_t, uint64_t>{});

    PrintContainer(ska::unordered_set<uint8_t>{}, ska::unordered_set<uint16_t>{}, ska::unordered_set<uint32_t>{}, ska::unordered_set<uint64_t>{});
    PrintContainer(ska::unordered_map<uint8_t, uint8_t>{}, ska::unordered_map<uint16_t, uint16_t>{}, ska::unordered_map<uint32_t, uint32_t>{},
                   ska::unordered_map<uint64_t, uint64_t>{});

    PrintNode(std::unordered_set<uint8_t>{}, std::unordered_set<uint16_t>{}, std::unordered_set<uint32_t>{}, std::unordered_set<uint64_t>{});
    PrintNode(std::unordered_map<uint8_t, uint8_t>{}, std::unordered_map<uint16_t, uint16_t>{}, std::unordered_map<uint32_t, uint32_t>{},
                   std::unordered_map<uint64_t, uint64_t>{});

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}