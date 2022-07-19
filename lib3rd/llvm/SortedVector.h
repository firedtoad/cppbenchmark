//
// Created by zhangwenhao.101 on 2022/7/19.
//

#ifndef LLVM_SORTEDVECTOR_H
#define LLVM_SORTEDVECTOR_H
#include <algorithm>
#include <functional>
#include <vector>

template <class T, class Compare = std::less<T>> struct sorted_vector {
//    static_assert(std::is_trivial<T>::value,"T must be trivial");
  static_assert(std::is_trivially_copyable<T>::value,
                "T must be trivially_copyable");
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  iterator begin() { return V.begin(); }
  iterator end() { return V.end(); }
  const_iterator begin() const { return V.begin(); }
  const_iterator end() const { return V.end(); }
  sorted_vector(const Compare &c = Compare()) : V(), cmp(c) {}
  template <class InputIterator>
  sorted_vector(InputIterator first, InputIterator last,
                const Compare &c = Compare())
      : V(first, last), cmp(c) {
    std::sort(begin(), end(), cmp);
  }
  iterator insert(const T &key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it == end() || cmp(key, *it))
      V.insert(it, key);
    return it;
  }
  iterator insert(T &&key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it == end() || cmp(key, *it))
      V.emplace(it, key);
    return it;
  }

  const_iterator find(const T &key) const {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    return it == end() || cmp(key, *it) ? end() : it;
  }

  const_iterator find( T &&key) const {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    return it == end() || cmp(key, *it) ? end() : it;
  }

  size_t erase(const T &key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it != end()) {
      V.erase(it);
      return 1;
    }
    return 0;
  }

  size_t erase(T &&key) {
    auto it = std::lower_bound(begin(), end(), key, cmp);
    if (it != end()) {
      V.erase(it);
      return 1;
    }
    return 0;
  }
  const_iterator erase(const_iterator &it) {
    if (it != end()) {
      return V.erase(it);
    }
    return end();
  }

private:
  std::vector<T> V;
  Compare cmp;
};
#endif // LLVM_SORTEDVECTOR_H
