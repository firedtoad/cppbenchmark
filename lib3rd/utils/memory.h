//
// Created by Administrator on 2023/06/04.
//

#ifndef BENCH_LIB3RD_UTILS_MEMORY_H_
#define BENCH_LIB3RD_UTILS_MEMORY_H_
#include "rss.h"
#include "symbol.h"
#include <memory>

template <class T> struct is_unique_ptr : std::false_type
{
};

template <class T, class D> struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type
{
};

template <class T> struct is_shared_ptr : std::false_type
{
};

template <class T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
{
};

template <class M, size_t N, bool reserve = true> void RSSMemoryMap()
{
    static rusage rusage;
    FillRSS(rusage);
    static M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        m[i] = i;
    }
    PrintUsage(rusage);
}

template <class M, size_t N, bool reserve = true> void RSSMemoryStringMap()
{
    static rusage rusage;
    FillRSS(rusage);
    static M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        m[std::to_string(i)] = i;
    }
    PrintUsage(rusage);
}

template <class M, size_t N, bool reserve = true> void RSSMemoryStringMapNoSSO()
{
    static rusage rusage;
    FillRSS(rusage);
    static M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        auto key          = std::to_string(i) + "123456789012345";
        m[std::move(key)] = i;
    }
    PrintUsage(rusage);
}

template <class M, size_t N, bool reserve = true> void RSSMemoryIntSet()
{
    static rusage rusage;
    FillRSS(rusage);
    static M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        m.insert(i);
    }
    PrintUsage(rusage);
}

template <class M, size_t N, bool reserve = true> void RSSMemoryStringSet()
{
    static rusage rusage;
    FillRSS(rusage);
    static M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        m.insert(std::to_string(i));
    }
    PrintUsage(rusage);
}

template <typename C, size_t N, bool reserve = true> void RSSMemoryLinear()
{
    static C c;
    using value_type = typename C::value_type;
    static rusage rusage;
    FillRSS(rusage);
    std::cout << demangle(typeid(c).name()) << " size " << N << " memory " << '\n';
    if constexpr (reserve)
    {
        c.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        if constexpr (is_unique_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.emplace_back(std::make_unique<T>());
        }
        else if constexpr (is_shared_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.emplace_back(std::make_shared<T>());
        }
        else
        {
            c.emplace_back();
        }
    }
    PrintUsage(rusage);
}

template <typename C, size_t N, bool reserve = true> void RSSMemoryForward()
{
    static C c;
    using value_type = typename C::value_type;
    static rusage rusage;
    FillRSS(rusage);
    std::cout << demangle(typeid(c).name()) << " size " << N << " memory " << '\n';
    if constexpr (reserve)
    {
        c.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        if constexpr (is_unique_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.push_front(std::make_unique<T>());
        }
        else if constexpr (is_shared_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.push_front(std::make_shared<T>());
        }
        else
        {
            c.push_front({});
        }
    }
    PrintUsage(rusage);
}

template <typename C, size_t N, bool reserve = true> void RSSMemoryMapInt()
{
    static C c;
    using value_type  = typename C::value_type;
    using second_type = typename value_type::second_type;
    static rusage rusage;
    FillRSS(rusage);
    std::cout << demangle(typeid(c).name()) << " size " << N << " memory " << '\n';
    if constexpr (reserve)
    {
        c.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        if constexpr (is_unique_ptr<second_type>::value)
        {
            using T = typename second_type::element_type;
            c.emplace(i, std::make_unique<T>());
        }
        else if constexpr (is_shared_ptr<second_type>::value)
        {
            using T = typename second_type::element_type;
            c.emplace(i, std::make_shared<T>());
        }
        else
        {
            c.emplace(i, second_type{});
        }
    }
    PrintUsage(rusage);
}

template <typename C, size_t N, bool reserve = true> void RSSMemoryMapString()
{
    static rusage rusage;
    FillRSS(rusage);
    static C c;
    using value_type  = typename C::value_type;
    using second_type = typename value_type::second_type;
    std::cout << demangle(typeid(c).name()) << " size " << N << " memory " << '\n';
    if constexpr (reserve)
    {
        c.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        if constexpr (is_unique_ptr<second_type>::value)
        {
            using T = typename second_type::element_type;
            c.emplace(std::to_string(i), std::make_unique<T>());
        }
        else if constexpr (is_shared_ptr<second_type>::value)
        {
            using T = typename second_type::element_type;
            c.emplace(std::to_string(i), std::make_shared<T>());
        }
        else
        {
            c.emplace(std::to_string(i), second_type{});
        }
    }
    PrintUsage(rusage);
}

template <typename C, size_t N, bool reserve = true> void RSSMemorySet()
{
    static C c;
    using value_type = typename C::value_type;
    static rusage rusage;
    FillRSS(rusage);
    std::cout << demangle(typeid(c).name()) << " size " << N << " memory " << '\n';
    if constexpr (reserve)
    {
        c.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        if constexpr (is_unique_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.emplace(std::make_unique<T>(i));
        }
        else if constexpr (is_shared_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.emplace(std::make_shared<T>(i));
        }
        else
        {
            c.emplace(value_type{i});
        }
    }
    PrintUsage(rusage);
}

template <typename C, size_t N, bool reserve = true> void RSSMemorySetString()
{
    static C c;
    using value_type = typename C::value_type;
    static rusage rusage;
    FillRSS(rusage);
    std::cout << demangle(typeid(c).name()) << " size " << N << " memory " << '\n';
    if constexpr (reserve)
    {
        c.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        if constexpr (is_unique_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.emplace(std::make_unique<T>(std::to_string(i)));
        }
        else if constexpr (is_shared_ptr<value_type>::value)
        {
            using T = typename value_type::element_type;
            c.emplace(std::make_shared<T>(std::to_string(i)));
        }
        else
        {
            c.emplace(value_type{std::to_string(i)});
        }
    }
    PrintUsage(rusage);
}

#endif // BENCH_LIB3RD_UTILS_MEMORY_H_
