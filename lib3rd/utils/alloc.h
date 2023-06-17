// Copyright 2023 The Division Authors.
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

#ifndef CPP_BENCHMARK_LIB3RD_UTILS_ALLOC_H_
#define CPP_BENCHMARK_LIB3RD_UTILS_ALLOC_H_
#include <iostream>
#include <malloc.h>
#include <new>
#include <unordered_map>

static size_t total_alloc    = 0;
static size_t current_used   = 0;
static size_t total_freed    = 0;
static size_t usable_size    = 0;
static size_t rss_size       = 0;
static size_t malloc_count   = 0;
static size_t calloc_count   = 0;
static size_t realloc_count  = 0;
static size_t free_count     = 0;
static size_t memalign_count = 0;
static size_t valloc_count   = 0;
static size_t pvalloc_count  = 0;

static bool size_alloc   = false;
static bool report_start = false;

std::unordered_map<void *, uint32_t> &GetSizeMap()
{
    static std::unordered_map<void *, uint32_t> ptr_size;
    return ptr_size;
}

struct AllocReport
{
    AllocReport()
    {
        total_alloc    = 0;
        current_used   = 0;
        total_freed    = 0;
        usable_size    = 0;
        rss_size       = 0;
        malloc_count   = 0;
        calloc_count   = 0;
        realloc_count  = 0;
        free_count     = 0;
        memalign_count = 0;
        valloc_count   = 0;
        pvalloc_count  = 0;

        size_alloc   = false;
        report_start = true;
    }

    AllocReport(std::string &&message) : AllocReport()
    {
        message_ = std::move(message);
    }

    ~AllocReport() noexcept
    {
        auto overhead_size = rss_size - total_alloc;
        std::cout << message_ << " logic allocate report" << '\n';
        std::cout << "total_alloc    : " << total_alloc << " / " << total_alloc / 1024 << " KB / " << (total_alloc >> 20) << " MB" << '\n';
        std::cout << "total_freed    : " << total_freed << " / " << total_freed / 1024 << " KB / " << (total_freed >> 20) << " MB" << '\n';
        std::cout << "current_used   : " << current_used << " / " << current_used / 1024 << " KB / " << (current_used >> 20) << " MB" << '\n';
        std::cout << "usable_size    : " << usable_size << " / " << usable_size / 1024 << " KB / " << (usable_size >> 20) << " MB" << '\n';
        std::cout << "overhead_size  : " << overhead_size << " / " << overhead_size / 1024 << " KB / " << (overhead_size >> 20) << " MB" << '\n';
        std::cout << "rss_size       : " << rss_size << " / " << rss_size / 1024 << " KB / " << (rss_size >> 20) << " MB" << '\n';
        std::cout << "malloc_count   : " << malloc_count << '\n';
        std::cout << "calloc_count   : " << calloc_count << '\n';
        std::cout << "realloc_count  : " << realloc_count << '\n';
        std::cout << "free_count     : " << free_count << '\n';
        std::cout << "memalign_count : " << memalign_count << '\n';
        std::cout << "valloc_count   : " << valloc_count << '\n';
        std::cout << "pvalloc_count  : " << pvalloc_count << '\n';
        std::cout << '\n';
        size_alloc   = true;
        report_start = false;
        GetSizeMap() = {};
    }
    std::string message_;
};

namespace std
{

void cache_size(size_t __size, void *p)
{
    size_alloc      = true;
    GetSizeMap()[p] = __size;
    size_alloc      = false;
}

size_t malloc_real_size(size_t usize, void *p)
{
#if defined(JE_MALLOC) || defined(TC_MALLOC) || defined(MI_MALLOC)
    auto real_size = usize;
#else
    auto real_size = *(size_t *)((char *)p - 8);
#endif
    return real_size;
}

void *hook_malloc(size_t __size)
{
    auto p = malloc(__size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    total_alloc += __size;
    current_used += __size;
    malloc_count++;
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    rss_size += real_size;
    usable_size += usize;
    cache_size(__size, p);
    return p;
}

void *hook_calloc(size_t __nmemb, size_t __size)
{
    auto p = calloc(__nmemb, __size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    total_alloc += __nmemb * __size;
    current_used += __nmemb * __size;
    calloc_count++;
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    usable_size += usize;
    rss_size += real_size;
    cache_size(__size, p);
    return p;
}

void *hook_realloc(void *__ptr, size_t __size)
{
    auto p = realloc(__ptr, __size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    total_alloc += __size - GetSizeMap()[__ptr];
    current_used += __size - GetSizeMap()[__ptr];
    realloc_count++;
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    usable_size += usize;
    rss_size += real_size;
    cache_size(__size, p);
    return p;
}

void *hook_reallocarray(void *__ptr, size_t __nmemb, size_t __size)
{
    auto p = reallocarray(__ptr, __nmemb, __size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    current_used += (__nmemb * __size) - GetSizeMap()[__ptr];
    total_alloc += (__nmemb * __size) - GetSizeMap()[__ptr];
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    usable_size += usize;
    rss_size += real_size;
    cache_size(__size * __nmemb, p);
    return p;
}

void hook_free(void *__ptr)
{
    free(__ptr);
    if (size_alloc || !report_start)
    {
        return;
    }
    total_freed += GetSizeMap()[__ptr];
    current_used -= GetSizeMap()[__ptr];
    free_count++;
}

void *hook_memalign(size_t __alignment, size_t __size)
{
    auto p = memalign(__alignment, __size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    //    auto align_size = ((__size + (__alignment - 1)) & ~(__alignment - 1));
    total_alloc += __size;
    current_used += __size;
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    usable_size += usize;
    rss_size += real_size;
    cache_size(__size, p);
    return p;
}
void *hook_valloc(size_t __size)
{
    auto p = valloc(__size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    total_alloc += __size;
    current_used += __size;
    valloc_count++;
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    usable_size += usize;
    rss_size += real_size;
    cache_size(__size, p);
    return p;
}

void *hook_pvalloc(size_t __size)
{
    auto p = pvalloc(__size);
    if (size_alloc || !report_start)
    {
        return p;
    }
    total_alloc += __size;
    current_used += __size;
    pvalloc_count++;
    auto usize     = malloc_usable_size(p);
    auto real_size = malloc_real_size(usize, p);
    usable_size += usize;
    rss_size += real_size;
    cache_size(__size, p);
    return p;
}

} // namespace std
void *operator new(size_t size)
{
    int *p = (int *)std::hook_malloc(size);
    return p;
}

void operator delete(void *p) noexcept
{
    std::hook_free(p);
}

void *operator new[](size_t size)
{
    void *p = std::hook_malloc(size);
    return p;
}

void operator delete[](void *p) noexcept
{
    std::hook_free(p);
}

#define malloc std::hook_malloc
#define calloc std::hook_calloc
#define realloc std::hook_realloc
#define reallocarray std::hook_reallocarray
#define free std::hook_free
#define memalign std::hook_memalign
#define valloc std::hook_valloc
#define pvalloc std::hook_pvalloc

#endif // CPP_BENCHMARK_LIB3RD_UTILS_ALLOC_H_
