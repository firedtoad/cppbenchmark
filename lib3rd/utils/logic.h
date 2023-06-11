//
// Created by Administrator on 2023/06/11.
//

#ifndef CPP_BENCHMARK_LIB3RD_UTILS_LOGIC_H_
#define CPP_BENCHMARK_LIB3RD_UTILS_LOGIC_H_
#include "alloc.h"
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

template <class M, size_t N, bool reserve = true> void LogicMemoryMap()
{
    AllocReport report{demangle(typeid(M).name())};
    M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        m[i] = i;
    }
}

template <class M, size_t N, bool reserve = true> void LogicMemoryStringMap()
{
    AllocReport report{demangle(typeid(M).name())};
    M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        m[std::to_string(i)] = i;
    }
}

template <class M, size_t N, bool reserve = true> void LogicMemoryStringMapNoSSO()
{
    AllocReport report{demangle(typeid(M).name())};
    M m;
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
}

template <class M, size_t N, bool reserve = true> void LogicMemoryIntSet()
{
    AllocReport report{demangle(typeid(M).name())};
    M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    std::cout << demangle(typeid(m).name()) << " memory " << '\n';
    for (size_t i = 0; i < N; i++)
    {
        m.insert(i);
    }
}

template <class M, size_t N, bool reserve = true> void LogicMemoryStringSet()
{
    AllocReport report{demangle(typeid(M).name())};
    M m;
    if constexpr (reserve)
    {
        m.reserve(N);
    }
    for (size_t i = 0; i < N; i++)
    {
        m.insert(std::to_string(i));
    }
}

template <typename C, size_t N, bool reserve = true> void LogicMemoryLinear()
{
    AllocReport report{demangle(typeid(C).name())};
    C c;
    using value_type = typename C::value_type;
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
}

template <typename C, size_t N, bool reserve = true> void LogicMemoryMapInt()
{
    AllocReport report{demangle(typeid(C).name())};
    C c;
    using value_type  = typename C::value_type;
    using second_type = typename value_type::second_type;
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
}

template <typename C, size_t N, bool reserve = true> void LogicMemoryMapString()
{
    AllocReport report{demangle(typeid(C).name())};
    C c;
    using value_type  = typename C::value_type;
    using second_type = typename value_type::second_type;
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
}

template <typename C, size_t N, bool reserve = true> void LogicMemorySet()
{
    AllocReport report{demangle(typeid(C).name())};
    C c;
    using value_type = typename C::value_type;
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
}

template <typename C, size_t N, bool reserve = true> void LogicMemorySetString()
{
    AllocReport report{demangle(typeid(C).name())};
    C c;
    using value_type = typename C::value_type;
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
}

#endif // CPP_BENCHMARK_LIB3RD_UTILS_LOGIC_H_
