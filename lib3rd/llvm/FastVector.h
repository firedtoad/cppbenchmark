//
// Created by Administrator on 2022/07/21.
//

#ifndef BENCH_LIB3RD_LLVM_FASTVECTOR_H_
#define BENCH_LIB3RD_LLVM_FASTVECTOR_H_
#include "tsl/robin_map.h"
#include <algorithm>
#include <functional>
#include <vector>

template <class T, class K = T> struct fast_vector
{
    using Vector = std::vector<T>;
    typedef typename Vector::iterator        iterator;
    typedef typename Vector::const_iterator  const_iterator;
    typedef typename Vector::reference       reference;
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
        m_f_index.reserve(size);
    }
    fast_vector() : m_data() {}
    template <class InputIterator> fast_vector(InputIterator first, InputIterator last) : m_data(first, last) {}

    iterator insert(const T &val)
    {
        auto key = *reinterpret_cast<const K *>(&val);
        auto it  = find(key);
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return m_data.insert(m_data.end(), val);
        }
        return it;
    }

    iterator insert(T &&val)
    {
        auto key = *reinterpret_cast<K *>(&val);
        auto it  = find(key);
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return m_data.insert(m_data.end(), std::forward<T>(val));
        }
        return it;
    }

    const_iterator find(const K &key) const
    {
        auto it = m_f_index.find(key);
        if (it != m_f_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    iterator find(const K &key)
    {
        auto it = m_f_index.find(key);
        if (it != m_f_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    reference operator[](const K &key)
    {
        auto it = find(key);
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return *m_data.emplace(m_data.end(), T{key});
        }
        return *it;
    }

    reference operator[](K &&key)
    {
        auto it = find(std::forward<T>(key));
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return *m_data.emplace(m_data.end(), T{key});
        }
        return *it;
    }

    size_t erase(const K &key)
    {
        auto it = m_f_index.find(key);
        if (it != m_f_index.end())
        {
            auto rit      = m_data.begin() + it->second;
            auto pos      = it->second;
            auto last_key = *reinterpret_cast<K *>(&m_data.back());
            std::swap(*rit, m_data.back());
            m_data.pop_back();
            m_f_index[last_key] = pos;
            m_f_index.erase(key);
            return 1;
        }
        return 0;
    }

    size_t erase(iterator &it)
    {
        if (it != m_data.end())
        {
            auto pos      = std::distance(m_data.begin(), it);
            auto last_key = *reinterpret_cast<K *>(&m_data.back());
            auto key      = m_data[pos];
            std::swap(*it, m_data.back());
            m_data.pop_back();
            m_f_index[last_key] = pos;
            m_f_index.erase(key);
            return 1;
        }
        return 0;
    }

  private:
    Vector                 m_data;
    tsl::robin_map<K, int> m_f_index; // forward index
};

template <class K, class V> struct fast_vector_map
{
    struct KVPair
    {
        K first;
        V second;
        KVPair() noexcept = default;
        KVPair(K k, V v) : first(std::move(k)), second(std::move(v)) {}
    };
    using Vector = std::vector<KVPair>;
    typedef typename Vector::iterator        iterator;
    typedef typename Vector::const_iterator  const_iterator;
    typedef typename Vector::reference       reference;
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
        m_f_index.reserve(size);
    }
    fast_vector_map() : m_data() {}
    template <class InputIterator> fast_vector_map(InputIterator first, InputIterator last) : m_data(first, last) {}

    iterator insert(const K &key, const V &val)
    {
        auto it = find(key);
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return m_data.insert(m_data.end(), std::make_pair(key, val));
        }
        return it;
    }

    iterator insert(K &&key, V &&val)
    {
        auto it = find(key);
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return m_data.insert(m_data.end(), std::make_pair(std::forward<K>(key), std::forward<V>(val)));
        }
        return it;
    }

    const_iterator find(const K &key) const
    {
        auto it = m_f_index.find(key);
        if (it != m_f_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    iterator find(const K &key)
    {
        auto it = m_f_index.find(key);
        if (it != m_f_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    V &operator[](const K &key)
    {
        auto it = find(key);
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return m_data.emplace(m_data.end(), key, V{})->second;
        }
        return it->second;
    }

    V &operator[](K &&key)
    {
        auto it = find(std::forward<K>(key));
        if (it == end())
        {
            uint32_t index = m_data.size();
            m_f_index[key] = index;
            return *m_data.emplace(m_data.end(), std::forward<K>(key), {});
        }
        return it;
    }

    size_t erase(const K &key)
    {
        auto it = m_f_index.find(key);
        if (it != m_f_index.end())
        {
            auto rit      = m_data.begin() + it->second;
            auto pos      = it->second;
            auto last_key = m_data.back().first;
            std::swap(*rit, m_data.back());
            m_data.pop_back();
            m_f_index[last_key] = pos;
            m_f_index.erase(key);
            return 1;
        }
        return 0;
    }

    size_t erase(iterator &it)
    {
        if (it != m_data.end())
        {
            auto pos      = std::distance(m_data.begin(), it);
            auto last_key = m_data.back().first;
            auto key      = m_data[pos];
            std::swap(*it, m_data.back());
            m_data.pop_back();
            m_f_index[last_key] = pos;
            m_f_index.erase(key);
            return 1;
        }
        return 0;
    }

  private:
    Vector                 m_data;
    tsl::robin_map<K, int> m_f_index; // forward index
};

#endif // BENCH_LIB3RD_LLVM_FASTVECTOR_H_
