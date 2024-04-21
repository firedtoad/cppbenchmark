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

#include <parallel_hashmap/btree.h>
#include <parallel_hashmap/phmap.h>
#include <tsl/sparse_map.h>
#include <tsl/sparse_set.h>

#include "utils/logic.h"
#include <benchmark/benchmark.h>
#include <cstring>
#include <iostream>
#include <openssl/rc5.h>
#include <variant>
#include <vector>
#include <iterator>
static void BenchMemCpy(benchmark::State &state)
{
    std::vector<char> cache;
    cache.resize(state.range(0));
    std::vector<char> dst;
    dst.resize(state.range(0));
    for (auto _ : state)
    {
        memcpy(dst.data(), cache.data(), cache.size());
        benchmark::DoNotOptimize(dst);
    }
}

static void BenchStdCopy(benchmark::State &state)
{
    std::vector<char> cache;
    cache.resize(state.range(0));
    std::vector<char> dst;
    dst.resize(state.range(0));
    for (auto _ : state)
    {
        std::copy(cache.begin(), cache.end(), dst.data());
        benchmark::DoNotOptimize(dst);
    }
}

BENCHMARK(BenchMemCpy)->Range(1, 1024);
BENCHMARK(BenchStdCopy)->Range(1, 1024);
struct WFGUID
{
    int64_t nData64;
    int64_t nHead64;
    bool operator!=(const WFGUID &o) const
    {
        return std::memcmp(this, &o, sizeof(*this));
    }
    bool operator==(const WFGUID &o) const
    {
        return std::memcmp(this, &o, sizeof(*this));
    }
};

struct Point3D
{
    float x; // x坐标
    float y; // y坐标
    float z; // z坐标
    bool operator==(const Point3D &o) const
    {
        return std::memcmp(this, &o, sizeof(*this));
    }
};

template <typename T> struct WFUnique
{
    WFUnique(WFUnique &&) noexcept = default;

    ~WFUnique() noexcept = default;
    WFUnique() noexcept  = default;

    WFUnique(T &&v) noexcept
    {
        //        if (v != T{})
        //        {
        val = std::make_unique<T>(std::forward<T>(v));
        //        }
    }

    WFUnique(const T &v) noexcept
    {
        //        if (v != T{})
        //        {
        val = std::make_unique<T>(v);
        //        }
    }

    WFUnique(const char *v) noexcept
    {
        //        if (v != T{})
        //        {
        val = std::make_unique<T>(v);
        //        }
    }

    WFUnique(const WFUnique &rhs) noexcept
    {
        *this = rhs;
    }

    WFUnique &operator=(const WFUnique &rhs) noexcept
    {
        if (rhs.val && val)
        {
            *val = *rhs.val;
            return *this;
        }
        if (rhs.val)
        {
            val = std::make_unique<T>(*rhs.val);
            return *this;
        }
        return *this;
    }

    WFUnique &operator=(WFUnique &&rhs) noexcept
    {
        if (rhs.val && val)
        {
            *val = std::move(*rhs.val);
            return *this;
        }
        val.reset(rhs.val.release());
        return *this;
    }

    bool operator==(const WFUnique &rhs) const
    {
        return val && rhs.val ? *val == *rhs.val : val == rhs.val;
    }

    const T &Value() const
    {
        return *val;
    }

    const T *Pointer() const
    {
        return val.get();
    }

  private:
    std::unique_ptr<T> val;
};

using WFData  = std::variant<std::monostate, int64_t, float, double, std::string, WFGUID, Point3D>;
using WFData1 = std::variant<std::monostate, int64_t, float, double, WFUnique<std::string>, WFUnique<WFGUID>, WFUnique<Point3D>>;

constexpr static size_t SIZE = 1 << 20;
struct NoDeleter
{
    void operator()(void *) {}
};

template<class T>
struct element_type : std::false_type
{
    using pointer = T *;
};

template<class T>
struct element_type<std::unique_ptr<T>> : std::true_type
{
    using pointer = typename std::unique_ptr<T>::element_type *;
};

template<class T>
struct element_type<std::shared_ptr<T>> : std::true_type
{
    using pointer = typename std::shared_ptr<T>::element_type *;
};

template<class T>
struct is_smart_ptr : std::false_type
{
};

template<class T>
struct is_smart_ptr<std::unique_ptr<T>> : std::true_type
{
};

template<class T>
struct is_smart_ptr<std::shared_ptr<T>> : std::true_type
{
};

template<class T>
struct element_ptr : std::false_type
{
    using pointer = T *;
};

template<class T>
struct element_ptr<std::unique_ptr<T>> : std::true_type
{
    using pointer = std::unique_ptr<T>;
};

template<class T>
struct element_ptr<std::shared_ptr<T>> : std::true_type
{
    using pointer = std::shared_ptr<T>;
};

template<typename T, typename TD, class WFMapOBJECT = phmap::btree_map<T, TD>>
class WFMapEx
{
  public:
    using MAPCALLBACK = std::function<bool(TD &)>;
    using Pointer = typename element_type<TD>::pointer;
    using Element = typename element_ptr<TD>::pointer;

    inline static int mnNFMapExCount;

    WFMapEx()
    {
        mnPos = 0;
        ++mnNFMapExCount;
    };

    virtual ~WFMapEx()
    {
        --mnNFMapExCount;
    };

    bool AddElement(const T &name, const TD &data, bool bSkipFindCheck = false)
    {
        if (bSkipFindCheck)
        {
            return mxObjectList.insert(typename WFMapOBJECT::value_type(name, data)).second;
        }

        typename WFMapOBJECT::iterator itr = mxObjectList.find(name);
        if (itr == mxObjectList.end())
        {
            mxObjectList.insert(typename WFMapOBJECT::value_type(name, data));
            return true;
        }

        return false;
    }

    void Reserve(size_t size)
    {
        mxObjectList.reserve(size);
    }

    bool SetElement(const T &name, const TD &data)
    {
        mxObjectList[name] = data;
        return true;
    }

    bool RemoveElement(const T &name)
    {
        return mxObjectList.erase(name) > 0;
    }

    Pointer GetElementNude(const T &name)
    {
        auto itr = mxObjectList.find(name);
        if (itr != mxObjectList.end())
        {
            return itr->second.get();
        }

        return nullptr;
    }

    Element GetElement(const T &name)
    {
        const auto itr = mxObjectList.find(name);
        if (itr != mxObjectList.end())
        {
            if constexpr (is_smart_ptr<TD>::value)
            {
                return itr->second;
            }
            else
            {
                return &itr->second;
            }
        }
        return nullptr;
    }

    Element GetElement(const T &name) const
    {
        const auto itr = mxObjectList.find(name);
        if (itr != mxObjectList.end())
        {
            if constexpr (is_smart_ptr<TD>::value)
            {
                return itr->second;
            }
            else
            {
                return &itr->second;
            }
        }
        return nullptr;
    }

    bool ExitElement(const T &name) const
    {
        return mxObjectList.find(name) != mxObjectList.end();
    }

    TD First()
    {
        if (mxObjectList.size() <= 0)
        {
            return nullptr;
        }

        mxObjectCurIter = mxObjectList.begin();
        if (mxObjectCurIter != mxObjectList.end())
        {
            return mxObjectCurIter->second;
        }

        return nullptr;
    }

    TD Next()
    {
        if (mxObjectCurIter == mxObjectList.end())
        {
            return nullptr;
        }

        ++mxObjectCurIter;
        if (mxObjectCurIter != mxObjectList.end())
        {
            return mxObjectCurIter->second;
        }

        return nullptr;
    }

    Pointer FirstNude(T &name)
    {
        if (mxObjectList.size() <= 0)
        {
            return NULL;
        }

        mxObjectCurIter = mxObjectList.begin();
        if (mxObjectCurIter != mxObjectList.end())
        {
            name = mxObjectCurIter->first;
            if constexpr (is_smart_ptr<TD>::value)
            {
                return mxObjectCurIter->second.get();
            }
            else
            {
                return &mxObjectCurIter->second;
            }
        }

        return NULL;
    }

    Pointer NextNude(T &name)
    {
        if (mxObjectCurIter == mxObjectList.end())
        {
            return NULL;
        }

        ++mxObjectCurIter;
        if (mxObjectCurIter != mxObjectList.end())
        {
            name = mxObjectCurIter->first;
            if constexpr (is_smart_ptr<TD>::value)
            {
                return mxObjectCurIter->second.get();
            }
            else
            {
                return &mxObjectCurIter->second;
            }
        }

        return NULL;
    }

    Pointer FirstNude()
    {
        if (mxObjectList.size() <= 0)
        {
            return NULL;
        }

        mxObjectCurIter = mxObjectList.begin();
        if (mxObjectCurIter != mxObjectList.end())
        {
            if constexpr (is_smart_ptr<TD>::value)
            {
                return mxObjectCurIter->second.get();
            }
            else
            {
                return &mxObjectCurIter->second;
            }
        }

        return NULL;
    }

    Pointer NextNude()
    {
        if (mxObjectCurIter == mxObjectList.end())
        {
            return NULL;
        }

        ++mxObjectCurIter;
        if (mxObjectCurIter != mxObjectList.end())
        {
            if constexpr (is_smart_ptr<TD>::value)
            {
                return mxObjectCurIter->second.get();
            }
            else
            {
                return &mxObjectCurIter->second;
            }
        }

        return NULL;
    }

    TD First(T &name)
    {
        if (mxObjectList.size() <= 0)
        {
            return nullptr;
        }

        mxObjectCurIter = mxObjectList.begin();
        if (mxObjectCurIter != mxObjectList.end())
        {
            name = mxObjectCurIter->first;
            return mxObjectCurIter->second;
        }

        return nullptr;
    }

    TD Next(T &name)
    {
        if (mxObjectCurIter == mxObjectList.end())
        {
            return nullptr;
        }

        mxObjectCurIter++;
        if (mxObjectCurIter != mxObjectList.end())
        {
            name = mxObjectCurIter->first;
            return mxObjectCurIter->second;
        }

        return nullptr;
    }

    bool DoAnyElement(const MAPCALLBACK &HandleMapCallBack)
    {
        for (auto iter = mxObjectList.begin(); iter != mxObjectList.end(); ++iter)
        {
            if (!HandleMapCallBack(iter->second))
            {
                return false;
            }
        }

        return true;
    }

    TD Suit()
    {
        T t;
        return Suit(t);
    }

    TD Suit(T &name)
    {
        // 不能使用迭代器那套，因为会打乱迭代器顺序
        if (Count() <= 0)
        {
            return nullptr;
        }

        mnPos = (mnPos + 1) % Count();
        typename WFMapOBJECT::iterator it = mxObjectList.begin();
        std::advance(it, mnPos);

        name = it->first;
        return it->second;
    }

    int Count() const
    {
        return int(mxObjectList.size());
    }

    bool ClearAll()
    {
        mxObjectList = {};
        return true;
    }

  private:
    int mnPos;
    WFMapOBJECT mxObjectList;
    typename WFMapOBJECT::iterator mxObjectCurIter;
};

int main(int argc, char **argv)
{

    WFMapEx<int, int> mapEx;
    mapEx.AddElement(1, 1);
    phmap::btree_map<int, int> bmap;
    bmap.erase(1);

    auto p = mapEx.GetElement(1);

    std::shared_ptr<int> pi(new int, [](void *) { std::cout << "here" << '\n'; });
    auto p2 = pi;

    std::cout << sizeof(WFData) << '\n';
    //    LogicMemoryLinear<std::vector<WFData>, SIZE>();
    {
        AllocReport report{demangle(typeid(std::vector<WFData>).name())};
        std::vector<WFData> data(SIZE);
        for (size_t i = 0; i < SIZE; i++)
        {
            if (i > (SIZE / 10))
            {
                data[i] = (int64_t)i;
            }
            else
            {
                data[i] = WFGUID{};
            }
        }
    }
    {
        WFUnique<std::string> sv = "124";

        WFData1 data1 = std::string{"abc"};
        //        WFData1 data1=std::string{""};

        const auto &str = std::get<4>(data1).Value();
        std::string ss  = "asadasda";
        data1           = ss;

        std::cout << "str " << str << str.length() << '\n';
        //        std::cout << "equals " << (int)(data1 == data2) << '\n';
        //        data2 = data1;
        data1 = WFGUID{1, 2};

        const WFGUID *pData = std::get_if<WFUnique<WFGUID>>(&data1)->Pointer();
        std::cout << pData->nData64 << pData->nHead64 << '\n';
        AllocReport report{demangle(typeid(std::vector<WFData1>).name())};
        std::vector<WFData1> data(SIZE);
        //        std::cout << SIZE / 10000 << '\n';
        auto sz = SIZE / 10;
        for (size_t i = 0; i < SIZE; i++)
        {
            if (i > sz)
            {
                data[i] = (int64_t)i;
            }
            else
            {
                data[i] = std::string{};
            }
        }
    }

    using VariantMap =
        std::variant<std::unordered_map<uint64_t, std::set<int>>, std::unordered_map<float, std::set<int>>, std::unordered_map<double, std::set<int>>,
                     std::unordered_map<std::string, std::set<int>>, std::unordered_map<WFGUID, std::set<int>>,
                     std::unordered_map<Point3D, std::set<int>>>; // 储存一列的索引的数据结构

    std::cout << sizeof(phmap::flat_hash_map<Point3D, phmap::flat_hash_set<int>>) << '\n';
    std::cout << sizeof(tsl::sparse_map<Point3D, tsl::sparse_set<int>>) << '\n';
    std::cout << sizeof(std::unordered_map<int, std::set<int>>) << '\n';
    std::cout << sizeof(std::unordered_map<std::string, std::set<int>>) << '\n';
    std::cout << sizeof(VariantMap) << '\n';
    //    benchmark::Initialize(&argc, argv);
    //    benchmark::RunSpecifiedBenchmarks();
    return 0;
}