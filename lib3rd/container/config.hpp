#ifndef CONFIG_TEMPLATE_HPP
#define CONFIG_TEMPLATE_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <stdint.h>

class CCsvBase
{
  public:
    virtual ~CCsvBase() = default;
    virtual bool Load() = 0;

    void AddCallCount() const
    {
        m_nCallCount++;
    }
    int GetCallCount() const
    {
        return m_nCallCount;
    }

  private:
    mutable int m_nCallCount = 0;
};

bool     LoadMember(uint8_t *pMember, uint8_t type, const std::string &name, const std::string &value);
uint64_t combineIndex(uint32_t key1, uint32_t key2);
uint32_t combineIndex(uint16_t key1, uint16_t key2);
uint64_t combineIndex(uint16_t key1, uint16_t key2, uint16_t key3);

template <typename T> struct ConfigTemplate : public CCsvBase
{
  public:
    ConfigTemplate() = default;

    explicit ConfigTemplate(std::string csvName) : m_strCsvName(std::move(csvName)) {}

    bool Load() override
    {
        return LoadFile(m_strCsvName.c_str());
    }

    bool LoadFile(const char *file)
    {
        auto strFile = std::string("config/") + file;
        return LoadConfig(strFile.c_str());
    }

    ~ConfigTemplate() override
    {
        Clear();
    }

    const T *begin() const
    {
        return reinterpret_cast<const T *>(data_.begin().base());
    }

    const T *end() const
    {
        return reinterpret_cast<const T *>(data_.end().base());
    }

    const T *GetByIndex(uint32_t index) const
    {
        const auto &it = index_.find(index);
        if (it != index_.end())
        {
            return GetByPos(it->second);
        }
        return nullptr;
    }

    const T *GetByPos(uint32_t pos) const
    {
        if (pos < count_)
        {
            return reinterpret_cast<const T *>(&data_[pos * SZ]);
        }
        return nullptr;
    }

    bool empty() const
    {
        return count_ == 0;
    }

    size_t size() const
    {
        return count_;
    }

    uint32_t Count() const
    {
        return count_;
    }

    const std::string &GetCsvName() const
    {
        return m_strCsvName;
    }

    void Clear()
    {
        for (size_t i = 0; i < count_; i++)
        {
            auto pObj = (T *)&data_[SZ * i];
            if (pObj)
            {
                pObj->~T();
            }
        }
        data_.clear();
        index_.clear();
    }

    explicit operator const std::vector<T> &() const
    {
        return *static_cast<std::vector<T> *>(&data_);
    }

  private:
    ConfigTemplate(const ConfigTemplate &)  = delete;
    ConfigTemplate(const ConfigTemplate &&) = delete;
    ConfigTemplate &operator=(const ConfigTemplate &) = delete;
    ConfigTemplate &operator=(const ConfigTemplate &&) = delete;

    bool LoadConfig(const char *file)
    {
        auto lines=0;
        size_t count = lines - 3;
        return true;
        std::vector<uint8_t>                   data;
        std::unordered_map<uint32_t, uint32_t> index;
        data.resize(count * SZ);
        index.reserve(count);
        uint8_t *pData = &data[0];
        uint8_t  idx   = 0;

        for (auto i = 3; i < lines; i++)
        {
            auto val = new (pData) T{};

            auto pMember = (uint8_t *)val;
//            for (auto &it : T::fields)
//            {
//                LoadMember(pMember + it.pos, it.type, it.name, oParser.GetField(i, it.name));
//            }
//
            auto id = *reinterpret_cast<uint32_t *>(pMember + T::fields[0].pos);

            index[id] = idx;
            pData += SZ;
            idx++;
        }
        data_  = std::move(data);
        index_ = std::move(index);
        count_ = count;
        return true;
    }
    const size_t                           SZ = sizeof(T);
    size_t                                 count_{};
    std::vector<uint8_t>                   data_{};
    std::unordered_map<uint32_t, uint32_t> index_{};
    std::string                            m_strCsvName{};
};

template <typename T> struct Iterator
{
    Iterator() = default;
    explicit Iterator(const void *const *first, const void *const *last)
    {
        begin_ = reinterpret_cast<T *>(first);
        end_   = reinterpret_cast<T *>(last);
    }
    T const *begin() const
    {
        return begin_;
    }
    T const *end() const
    {
        return end_;
    }
    T *begin_{};
    T *end_{};
};

template <typename T, uint16_t pos1, uint16_t pos2> struct Index2F
{
    static_assert(pos1 != pos2, "second index same field");
    explicit Index2F(const ConfigTemplate<T> &config) : config_(config) {}

    void Clear()
    {
        std::unordered_map<uint32_t, std::vector<const void *>> emptyIndex;
        std::unordered_map<uint64_t, uint32_t>                  emptyPos;
        indexVal_.swap(emptyIndex);
        indexPos_.swap(emptyPos);
    }

    void BuildIndex()
    {
        Clear();
        auto pos = 0;
        for (auto &it : config_)
        {
            auto p = (uint8_t *)&it;

            uint32_t index1 = *reinterpret_cast<uint32_t *>(p + pos1);
            uint32_t index2 = *reinterpret_cast<uint32_t *>(p + pos2);

            auto combine64 = combineIndex(index1, index2);

            indexPos_[combine64] = pos++;
            indexVal_[index1].emplace_back(&it);
        }
    }

    const T *GetByIndex(uint32_t index1, uint32_t index2) const
    {
        auto        combine = combineIndex(index1, index2);
        const auto &it      = indexPos_.find(combine);
        if (it != indexPos_.end())
        {
            return config_.GetByPos(it->second);
        }
        return nullptr;
    }

    const Iterator<const T * const> GetByIndex(uint32_t index) const
    {
        const auto &it = indexVal_.find(index);
        if (it != indexVal_.end() && it->second.size() > 0)
        {
            return Iterator<const T * const>{it->second.begin().base(), it->second.end().base()};
        }
        return {};
    }

  private:
    std::unordered_map<uint32_t, std::vector<const void *>> indexVal_{};
    std::unordered_map<uint64_t, uint32_t>                  indexPos_{};
    const ConfigTemplate<T>                                &config_{};
};

template <typename T, uint16_t pos1, uint16_t pos2, uint16_t pos3> struct Index3F
{
    static_assert(pos1 != pos2, "third index same field");
    static_assert(pos2 != pos3, "third index same field");
    explicit Index3F(ConfigTemplate<T> &config) : config_(config) {}
    void Clear()
    {
        std::unordered_map<uint32_t, std::vector<const void *>> emptyIndex;
        std::unordered_map<uint64_t, uint32_t>                  emptyPos;
        indexVal_.swap(emptyIndex);
        indexPos_.swap(emptyPos);
    }

    void BuildIndex()
    {
        Clear();
        auto pos = 0;
        for (auto &it : config_)
        {
            auto p = (uint8_t *)&it;

            uint16_t index1 = *reinterpret_cast<uint16_t *>(p + pos1);
            uint16_t index2 = *reinterpret_cast<uint16_t *>(p + pos2);
            uint16_t index3 = *reinterpret_cast<uint16_t *>(p + pos3);

            auto combine64 = combineIndex(index1, index2, index3);
            auto combine32 = combineIndex(index1, index2);

            indexPos_[combine64] = pos;
            indexVal_[combine32].emplace_back(&it);
        }
    }

    const T *GetByIndex(uint16_t index1, uint16_t index2, uint16_t index3) const
    {
        auto combine = combineIndex(index1, index2, index3);

        const auto &it = indexPos_.find(combine);
        if (it != indexPos_.end())
        {
            return config_.GetByPos(it->second);
        }
        return nullptr;
    }

    Iterator<const T * const> GetByIndex(uint16_t index1, uint16_t index2) const
    {
        auto        combine = combineIndex(index1, index2);
        const auto &it      = indexVal_.find(combine);
        if (it != indexVal_.end() && it->second.size() > 0)
        {
            return Iterator<const T * const>{it->second.begin().base(), it->second.end().base()};
        }
        return {};
    }

  private:
    std::unordered_map<uint32_t, std::vector<const void *>> indexVal_{};
    std::unordered_map<uint64_t, uint32_t>                  indexPos_{};
    const ConfigTemplate<T>                                &config_{};
};

#endif // CONFIG_TEMPLATE_HPP
