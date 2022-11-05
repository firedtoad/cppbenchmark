//
// Created by zhangwenhao.101 on 2022/9/21.
//
#include <cxxabi.h>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

int main(int argc, char *argv[])
{
    PrintVector(std::vector<uint8_t>{}, std::vector<uint16_t>{}, std::vector<uint32_t>{}, std::vector<uint64_t>{});
    PrintVector(std::deque<uint8_t>{}, std::deque<uint16_t>{}, std::deque<uint32_t>{}, std::deque<uint64_t>{});
    PrintVector(std::queue<uint8_t>{}, std::queue<uint16_t>{}, std::queue<uint32_t>{}, std::queue<uint64_t>{});
    PrintList(std::_List_node<uint8_t>{}, std::_List_node<uint16_t>{}, std::_List_node<uint32_t>{}, std::_List_node<uint64_t>{});
    PrintList(std::_Rb_tree_node<uint8_t>{}, std::_Rb_tree_node<uint16_t>{}, std::_Rb_tree_node<uint32_t>{}, std::_Rb_tree_node<uint64_t>{});
    PrintList(std::__detail::_Hash_node<std::pair<uint8_t ,uint8_t>,false>{}, std::_Rb_tree_node<uint16_t>{}, std::_Rb_tree_node<uint32_t>{}, std::_Rb_tree_node<uint64_t>{});
    PrintNode(std::set<uint8_t>{}, std::set<uint16_t>{}, std::set<uint32_t>{}, std::set<uint64_t>{});
    PrintNode(std::map<uint8_t, uint8_t>{}, std::map<uint16_t, uint16_t>{}, std::map<uint32_t, uint32_t>{}, std::map<uint64_t, uint64_t>{});
    PrintNode(std::unordered_set<uint8_t>{}, std::unordered_set<uint16_t>{}, std::unordered_set<uint32_t>{}, std::unordered_set<uint64_t>{});
    PrintNode(std::unordered_map<uint8_t, uint8_t>{}, std::unordered_map<uint16_t, uint16_t>{}, std::unordered_map<uint32_t, uint32_t>{},
              std::unordered_map<uint64_t, uint64_t>{});
    return 0;
}
