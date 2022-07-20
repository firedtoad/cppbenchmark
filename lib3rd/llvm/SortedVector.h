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

template <class T, class Compare = std::less<T>> struct sorted_vector {
  static_assert(std::is_trivially_copyable<T>::value,
                "T must be trivially_copyable");
  static_assert(sizeof(T) <= 16, "sizeof T must less than 16");
  using Vector = std::vector<T, __gnu_cxx::__pool_alloc<T>>;

  typedef typename Vector::iterator iterator;
  typedef typename Vector::const_iterator const_iterator;
  typedef typename Vector::reference reference;
  typedef typename Vector::const_reference const_reference;
  iterator begin() { return m_data.begin(); }
  iterator end() { return m_data.end(); }
  const_iterator begin() const { return m_data.begin(); }
  const_iterator end() const { return m_data.end(); }
  sorted_vector(const Compare &c = Compare()) : m_data(), cmp(c) {}
  template <class InputIterator>
  sorted_vector(InputIterator first, InputIterator last,
                const Compare &c = Compare())
      : m_data(first, last), cmp(c) {
    std::sort(begin(), end(), cmp);
  }
  iterator insert(const T &key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it == end() || cmp(key, *it)) {
      return m_data.insert(it, key);
    }
    return it;
  }

  iterator insert(T &&key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it == end() || cmp(key, *it)) {
      return m_data.emplace(it, key);
    }
    return it;
  }

  const_iterator find(const T &key) const {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    return it == end() || cmp(key, *it) ? end() : it;
  }

  iterator find(T &&key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    return it == end() || cmp(key, *it) ? end() : it;
  }

  const_reference operator[](const T &key) const { return *insert(key); }

  reference operator[](T &&key) { return *insert(key); }

  size_t erase(const T &key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it != end()) {
      m_data.erase(it);
      return 1;
    }
    return 0;
  }

  size_t erase(T &&key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it != end()) {
      m_data.erase(it);
      return 1;
    }
    return 0;
  }
  const_iterator erase(const_iterator &it) {
    if (it != end()) {
      return m_data.erase(it);
    }
    return end();
  }

  template <typename F> const_iterator erase_if(F &&f) {
    return m_data.erase(std::remove_if(m_data.begin, m_data.end(), f),
                        m_data.end());
  }

private:
  Vector m_data;
  Compare cmp;
};
#endif // LLVM_SORTED_VECTOR_H
