//
// Created by zhangwenhao.101 on 2022/7/19.
//

#ifndef LLVM_SORTED_VECTOR_H
#define LLVM_SORTED_VECTOR_H
#include "SmallVector.h"
#include <algorithm>
#include <ext/pool_allocator.h>
#include <functional>
#include <vector>

template <class T, class Compare = std::less<T>> struct sorted_vector
{
    static_assert(std::is_trivially_copyable<T>::value, "T must be trivially_copyable");
    using Vector = std::vector<T>;

    typedef typename Vector::iterator iterator;
    typedef typename Vector::const_iterator const_iterator;
    typedef typename Vector::reference reference;
    typedef typename Vector::const_reference const_reference;
    iterator begin()
    {
        return m_data.begin();
    }
    iterator end()
    {
        return m_data.end();
    }
    const_iterator begin() const
    {
        return m_data.begin();
    }
    const_iterator end() const
    {
        return m_data.end();
    }
    size_t empty()
    {
        return m_data.empty();
    }
    size_t size()
    {
        return m_data.size();
    }
    void reserve(size_t size)
    {
        m_data.reserve(size);
    }

    sorted_vector(const Compare &c = Compare()) : m_data(), cmp(c) {}
    template <class InputIterator> sorted_vector(InputIterator first, InputIterator last, const Compare &c = Compare()) : m_data(first, last), cmp(c)
    {
        std::sort(begin(), end(), cmp);
    }
    iterator insert(const T &key)
    {
        auto it = std::lower_bound(begin(), end(), key, cmp);
        if (it == end() || cmp(key, *it))
        {
            return m_data.insert(it, key);
        }
        return it;
    }

    iterator insert(T &&key)
    {
        auto it = std::lower_bound(begin(), end(), std::forward<T>(key), cmp);
        if (it == end() || cmp(key, *it))
        {
            return m_data.emplace(it, std::forward<T>(key));
        }
        return it;
    }

    const_iterator find(const T &key) const
    {
        auto it = std::lower_bound(begin(), end(), key, cmp);
        return it == end() || cmp(key, *it) ? end() : it;
    }

    iterator find(T &&key)
    {
        auto it = std::lower_bound(begin(), end(), std::forward<T>(key), cmp);
        return it == end() || cmp(std::forward<T>(key), *it) ? end() : it;
    }

    reference operator[](const T &key)
    {
        return *insert(key);
    }

    reference operator[](T &&key)
    {
        return *insert(key);
    }

    size_t erase(const T &key)
    {
        auto it = std::lower_bound(begin(), end(), key, cmp);
        if (it != end())
        {
            m_data.erase(it);
            return 1;
        }
        return 0;
    }

    size_t erase(T &&key)
    {
        auto it = std::lower_bound(begin(), end(), std::forward<T>(key), cmp);
        if (it != end())
        {
            m_data.erase(it);
            return 1;
        }
        return 0;
    }

    const_iterator erase(const_iterator &it)
    {
        if (it != end())
        {
            return m_data.erase(it);
        }
        return end();
    }

    template <typename F> const_iterator erase_if(F &&f)
    {
        return m_data.erase(std::remove_if(m_data.begin, m_data.end(), std::forward<F>(f)), m_data.end());
    }

  private:
    Vector m_data;
    Compare cmp;
};

template <class K, class V, class Compare = std::less<K>> struct sorted_vector_map
{
    struct KVPair
    {
        K first;
        V second;
        KVPair() noexcept = default;
        KVPair(K k, V v) : first(std::move(k)), second(std::move(v)) {}
    };

    static_assert(std::is_trivial<V>::value, "V must be trivially_copyable");
    static_assert(sizeof(V) <= 16, "sizeof V must less than 16");
    using Vector = std::vector<KVPair>;

    typedef typename Vector::iterator iterator;
    typedef typename Vector::const_iterator const_iterator;
    typedef typename Vector::reference reference;
    typedef typename Vector::const_reference const_reference;
    iterator begin()
    {
        return m_data.begin();
    }
    iterator end()
    {
        return m_data.end();
    }
    const_iterator begin() const
    {
        return m_data.begin();
    }
    const_iterator end() const
    {
        return m_data.end();
    }
    size_t empty()
    {
        return m_data.empty();
    }
    size_t size()
    {
        return m_data.size();
    }
    void reserve(size_t size)
    {
        m_data.reserve(size);
    }

    sorted_vector_map(const Compare &c = Compare()) : m_data(), cmp(c) {}
    template <class InputIterator>
    sorted_vector_map(InputIterator first, InputIterator last, const Compare &c = Compare()) : m_data(first, last), cmp(c)
    {
        std::sort(begin(), end(), [this](const auto &p1, const auto &p2) { return cmp(p1.first, p2); });
    }

    iterator insert(const K &key, const V &val)
    {
        auto it = std::lower_bound(begin(), end(), key, [this](const auto &p1, const auto &p2) { return cmp(p1.first, p2); });
        if (it == end() || cmp(key, it->first))
        {
            return m_data.emplace(it, key, val);
        }
        return it;
    }

    iterator insert(K &&key, V &&val)
    {
        auto it = std::lower_bound(begin(), end(), std::forward<K>(key), [this](const auto &p1, const auto &p2) { return cmp(p1.first, p2); });
        if (it == end() || cmp(key, it->first))
        {
            return m_data.emplace(it, std::forward<K>(key), std::forward<V>(val));
        }
        return it;
    }

    const_iterator find(const K &key) const
    {
        auto it = std::lower_bound(begin(), end(), key, [this](const auto &p1, const auto &p2) { return cmp(p1.first, p2); });
        return it == end() || cmp(key, it->first) ? end() : it;
    }

    iterator find(K &&key)
    {
        auto it = std::lower_bound(begin(), end(), std::forward<K>(key), [this](const auto &p1, const auto &p2) { return cmp(p1.first, p2); });
        return it == end() || cmp(std::forward<K>(key), it->first) ? end() : it;
    }

    V &operator[](const K &key)
    {
        return insert(key, {})->second;
    }

    V &operator[](K &&key)
    {
        return insert(std::forward<K>(key), {})->second;
    }

    size_t erase(const K &key)
    {
        auto it = find(key);
        if (it != end())
        {
            m_data.erase(it);
            return 1;
        }
        return 0;
    }

    size_t erase(K &&key)
    {
        auto it = find(std::forward<K>(key));
        if (it != end())
        {
            m_data.erase(it);
            return 1;
        }
        return 0;
    }

    const_iterator erase(const_iterator &it)
    {
        if (it != end())
        {
            return m_data.erase(it);
        }
        return end();
    }

    template <typename F> const_iterator erase_if(F &&f)
    {
        return m_data.erase(std::remove_if(m_data.begin, m_data.end(), std::forward<F>(f)), m_data.end());
    }

  private:
    Vector m_data;
    Compare cmp;
};

#endif // LLVM_SORTED_VECTOR_H
