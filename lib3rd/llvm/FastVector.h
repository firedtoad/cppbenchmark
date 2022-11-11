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

#ifndef BENCH_LIB3RD_LLVM_FASTVECTOR_H_
#define BENCH_LIB3RD_LLVM_FASTVECTOR_H_
#include "parallel_hashmap/phmap.h"
#include <algorithm>
#include <functional>
#include <vector>

template <class T, class K = T, class FastMap = phmap::flat_hash_map<K, uint32_t>> struct fast_vector
{
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
        m_index.reserve(size);
    }
    void clear()
    {
        m_data.clear();
        m_index.clear();
    }
    fast_vector() : m_data() {}
    template <class InputIterator> fast_vector(InputIterator first, InputIterator last) : m_data(first, last)
    {
        m_index.reserve(m_data.size());
        for (auto i = 0; i < m_data.size(); i++)
        {
            m_index[m_data[i].first] = i;
        }
    }

    iterator insert(const T &val)
    {
        auto key    = *reinterpret_cast<const K *>(&val);
        auto it     = m_index.insert(std::make_pair(key, 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(val);
            index = m_data.size() - 1;
        }
        return m_data.begin() + index;
    }

    iterator insert(T &&val)
    {
        auto key    = *reinterpret_cast<const K *>(&val);
        auto it     = m_index.insert(std::make_pair(key, 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(std::forward<T>(val));
            index = m_data.size() - 1;
        }
        return m_data.begin() + index;
    }

    const_iterator find(const K &key) const
    {
        auto it = m_index.find(key);
        if (it != m_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    iterator find(const K &key)
    {
        auto it = m_index.find(key);
        if (it != m_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    reference operator[](const K &key)
    {
        auto it     = m_index.insert(std::make_pair(key, 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(key);
            index = m_data.size() - 1;
        }
        return m_data[index];
    }

    reference operator[](K &&key)
    {
        auto it     = m_index.insert(std::make_pair(std::forward<K>(key), 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(std::forward<K>(key));
            index = m_data.size() - 1;
        }
        return m_data[index];
    }

    size_t erase(const K &key)
    {
        auto it = m_index.find(key);
        if (it != m_index.end())
        {
            auto rit      = m_data.begin() + it->second;
            auto pos      = it->second;
            auto last_key = *reinterpret_cast<K *>(&m_data.back());
            std::swap(*rit, m_data.back());
            m_data.pop_back();
            m_index[last_key] = pos;
            m_index.erase(key);
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
            m_index[last_key] = pos;
            m_index.erase(key);
            return 1;
        }
        return 0;
    }

    void pop_back()
    {
        auto pos = m_index.find(m_data.back().first);
        m_index.erase(pos);
        m_data.pop_back();
    }

  private:
    Vector m_data;
    FastMap m_index; // index
};

template <class K, class V, class FastMap = phmap::flat_hash_map<K, uint32_t>> struct fast_vector_map
{
    using Vector = std::vector<std::pair<K, V>>;
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
        m_index.reserve(size);
    }
    void clear()
    {
        m_data.clear();
        m_index.clear();
    }
    fast_vector_map() : m_data() {}
    template <class InputIterator> fast_vector_map(InputIterator first, InputIterator last) : m_data(first, last)
    {
        m_index.reserve(m_data.size());
        for (auto i = 0; i < m_data.size(); i++)
        {
            m_index[m_data[i].first] = i;
        }
    }

    iterator insert(const K &key, const V &val)
    {
        auto it     = m_index.insert(std::make_pair(key, 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(key, V{});
            index = m_data.size() - 1;
        }
        return m_data.begin() + index;
    }

    iterator insert(K &&key, V &&val)
    {
        auto it     = m_index.insert(std::make_pair(std::forward<K>(key), 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(std::forward<K>(key), V{});
            index = m_data.size() - 1;
        }
        return m_data.begin() + index;
    }

    const_iterator find(const K &key) const
    {
        auto it = m_index.find(key);
        if (it != m_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    iterator find(const K &key)
    {
        auto it = m_index.find(key);
        if (it != m_index.end())
        {
            return m_data.begin() + it->second;
        }
        return m_data.end();
    }

    V &operator[](const K &key)
    {
        auto p      = std::make_pair(key, 0);
        auto it     = m_index.insert(p);
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(key, V{});
            index = m_data.size() - 1;
        }
        return m_data[index].second;
    }

    V &operator[](K &&key)
    {
        auto it     = m_index.insert(std::make_pair(key, 0));
        auto &index = it.first->second;
        if (it.second)
        {
            m_data.emplace_back(std::forward<K>(key), V{});
            index = m_data.size() - 1;
        }
        return m_data[index].second;
    }

    size_t erase(const K &key)
    {
        auto it = m_index.find(key);
        if (it != m_index.end())
        {
            auto rit      = m_data.begin() + it->second;
            auto pos      = it->second;
            auto last_key = m_data.back().first;
            std::swap(*rit, m_data.back());
            m_data.pop_back();
            m_index[last_key] = pos;
            m_index.erase(key);
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
            m_index[last_key] = pos;
            m_index.erase(key);
            return 1;
        }
        return 0;
    }
    void pop_back()
    {
        auto pos = m_index.find(m_data.back().first);
        m_index.erase(pos);
        m_data.pop_back();
    }

  private:
    Vector m_data;
    FastMap m_index; // forward index
};

#endif // BENCH_LIB3RD_LLVM_FASTVECTOR_H_
