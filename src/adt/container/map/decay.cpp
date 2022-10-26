//
// Created by Administrator on 2022/01/15.
//

#include <benchmark/benchmark.h>
#include <cxxabi.h>
#include <iostream>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>
#include <unordered_map>
#include <llvm/ADT/simple_ilist.h>
std::string demangle(const char *name)
{
    int status                       = -4;
    char *res                        = abi::__cxa_demangle(name, NULL, NULL, &status);
    const char *const demangled_name = (status == 0) ? res : name;
    std::string ret_val(demangled_name);
    free(res);
    return ret_val;
}
template <typename... T> void PrintVector(T &&...t)
{
    (..., (std::cout << demangle(typeid(t).name()) << " size " << sizeof(typename T::value_type) << '\n'));
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
template <typename T> struct KeyInfo
{
    static inline T getEmptyKey()
    {
        return ~0;
    }
    static inline T getTombstoneKey()
    {
        return ~0 - 1;
    }
    static unsigned getHashValue(const T &Val)
    {
        return 1;
    }
    static bool isEqual(const T &LHS, const T &RHS)
    {
        return LHS == RHS;
    }
};

static void BM_unmap(benchmark::State &state)
{
    std::unordered_map<int, int> unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            unmap[i] = i;
        }
    }
}

static void BM_decay_map(benchmark::State &state)
{
    std::unordered_map<int, int, Hasher> decay_unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            decay_unmap[i] = i;
        }
    }
}

BENCHMARK(BM_unmap);
BENCHMARK(BM_decay_map);

static void BM_DenseMap(benchmark::State &state)
{
    llvm::DenseMap<int, int> unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            unmap[i] = i;
        }
    }
}

static void BM_Decay_DenseMap(benchmark::State &state)
{
    llvm::DenseMap<int, int, KeyInfo<int>> decay_unmap;
    for (auto _ : state)
    {
        for (int i = 0; i < 10000; i++)
        {
            decay_unmap[i] = i;
        }
    }
}

BENCHMARK(BM_DenseMap);
BENCHMARK(BM_Decay_DenseMap);

struct LNode : public llvm::ilist_node<LNode, llvm::ilist_tag<LNode>>
{
    char c;
};

int main(int argc, char **argv)
{

    PrintList(llvm::detail::DenseSetPair<uint8_t>{}, llvm::detail::DenseSetPair<uint16_t>{}, llvm::detail::DenseSetPair<uint32_t>{},
              llvm::detail::DenseSetPair<uint64_t>{});
    PrintContainer(llvm::DenseMap<uint8_t, uint8_t, KeyInfo<uint8_t>>{}, llvm::DenseMap<uint16_t, uint16_t, KeyInfo<uint16_t>>{},
                   llvm::DenseMap<uint32_t, uint32_t, KeyInfo<uint32_t>>{}, llvm::DenseMap<uint64_t, uint64_t, KeyInfo<uint64_t>>{});
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}