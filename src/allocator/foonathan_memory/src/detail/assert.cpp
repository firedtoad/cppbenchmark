// Copyright (C) 2015-2020 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include "detail/assert.hpp"

#if FOONATHAN_HOSTED_IMPLEMENTATION
#include <cstdio>
#endif

#include <cstdlib>

#include "error.hpp"

using namespace foonathan::memory;
using namespace detail;

void detail::handle_failed_assert(const char* msg, const char* file, int line,
                                  const char* fnc) noexcept
{
#if FOONATHAN_HOSTED_IMPLEMENTATION
    std::fprintf(stderr, "[%s] Assertion failure in function %s (%s:%d): %s.\n",
                 FOONATHAN_MEMORY_LOG_PREFIX, fnc, file, line, msg);
#endif
    std::abort();
}

void detail::handle_warning(const char* msg, const char* file, int line,
                            const char* fnc) noexcept
{
#if FOONATHAN_HOSTED_IMPLEMENTATION
    std::fprintf(stderr, "[%s] Warning triggered in function %s (%s:%d): %s.\n",
                 FOONATHAN_MEMORY_LOG_PREFIX, fnc, file, line, msg);
#endif
}
