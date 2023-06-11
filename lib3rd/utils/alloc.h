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
static size_t malloc_count   = 0;
static size_t calloc_count   = 0;
static size_t realloc_count  = 0;
static size_t free_count     = 0;
static size_t memalign_count = 0;
static size_t valloc_count   = 0;
static size_t pvalloc_count  = 0;

static bool size_alloc = false;

static std::unordered_map<void *, uint32_t> ptr_size;
struct AllocReport
{
    AllocReport()
    {
        total_alloc    = 0;
        current_used   = 0;
        total_freed    = 0;
        malloc_count   = 0;
        calloc_count   = 0;
        realloc_count  = 0;
        free_count     = 0;
        memalign_count = 0;
        valloc_count   = 0;
        pvalloc_count  = 0;
        size_alloc     = false;
    }

    AllocReport(std::string &&message) : AllocReport()
    {
        message_ = std::move(message);
    }

    ~AllocReport() noexcept
    {
        std::cout << message_ << " logic allocate report" << '\n';
        std::cout << "total_alloc    : " << total_alloc << " / " << total_alloc / 1024 << " KB / " << (total_alloc >> 20) << " MB" << '\n';
        std::cout << "total_freed    : " << total_freed << " / " << total_freed / 1024 << " KB / " << (total_freed >> 20) << " MB" << '\n';
        std::cout << "current_used   : " << current_used << " / " << current_used / 1024 << " KB / " << (current_used >> 20) << " MB" << '\n';
        std::cout << "malloc_count   : " << malloc_count << '\n';
        std::cout << "calloc_count   : " << calloc_count << '\n';
        std::cout << "realloc_count  : " << realloc_count << '\n';
        std::cout << "free_count     : " << free_count << '\n';
        std::cout << "memalign_count : " << memalign_count << '\n';
        std::cout << "valloc_count   : " << valloc_count << '\n';
        std::cout << "pvalloc_count  : " << pvalloc_count << '\n';
        std::cout << '\n';
        size_alloc = true;
        ptr_size   = {};
    }
    std::string message_;
};

namespace std
{

void cache_size(size_t __size, void *p)
{
    size_alloc  = true;
    ptr_size[p] = __size;
    size_alloc  = false;
}

void *hook_malloc(size_t __size)
{
    auto p = malloc(__size);
    if (size_alloc)
    {
        return p;
    }
    total_alloc += __size;
    current_used += __size;
    malloc_count++;
    cache_size(__size, p);
    return p;
}

void *hook_calloc(size_t __nmemb, size_t __size)
{
    auto p = calloc(__nmemb, __size);
    if (size_alloc)
    {
        return p;
    }
    total_alloc += __nmemb * __size;
    current_used += __nmemb * __size;
    calloc_count++;
    cache_size(__size, p);
    return p;
}

void *hook_realloc(void *__ptr, size_t __size)
{
    auto p = realloc(__ptr, __size);
    if (size_alloc)
    {
        return p;
    }
    total_alloc += __size - ptr_size[__ptr];
    current_used += __size - ptr_size[__ptr];
    realloc_count++;
    cache_size(__size, p);
    return p;
}

void *hook_reallocarray(void *__ptr, size_t __nmemb, size_t __size)
{
    auto p = reallocarray(__ptr, __nmemb, __size);
    if (size_alloc)
    {
        return p;
    }
    current_used += (__nmemb * __size) - ptr_size[__ptr];
    total_alloc += (__nmemb * __size) - ptr_size[__ptr];
    cache_size(__size * __nmemb, p);
    return p;
}

void hook_free(void *__ptr)
{
    free(__ptr);
    if (size_alloc)
    {
        return;
    }
    total_freed += ptr_size[__ptr];
    current_used -= ptr_size[__ptr];
    free_count++;
}

void *hook_memalign(size_t __alignment, size_t __size)
{
    auto p = memalign(__alignment, __size);
    if (size_alloc)
    {
        return p;
    }
    //    auto align_size = ((__size + (__alignment - 1)) & ~(__alignment - 1));
    total_alloc += __size;
    current_used += __size;
    cache_size(__size, p);
    return p;
}
void *hook_valloc(size_t __size)
{
    auto p = valloc(__size);
    if (size_alloc)
    {
        return p;
    }
    total_alloc += __size;
    current_used += __size;
    valloc_count++;
    cache_size(__size, p);
    return p;
}

void cache_size(size_t __size, const void *p);
void *hook_pvalloc(size_t __size)
{
    auto p = pvalloc(__size);
    if (size_alloc)
    {
        return p;
    }
    total_alloc += __size;
    current_used += __size;
    pvalloc_count++;
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
