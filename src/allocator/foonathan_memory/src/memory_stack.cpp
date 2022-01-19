// Copyright (C) 2015-2020 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include "memory_stack.hpp"

#include "debugging.hpp"

using namespace foonathan::memory;

void detail::memory_stack_leak_handler::operator()(std::ptrdiff_t amount)
{
    get_leak_handler()({FOONATHAN_MEMORY_LOG_PREFIX "::memory_stack", this}, amount);
}

#if FOONATHAN_MEMORY_EXTERN_TEMPLATE
template class foonathan::memory::memory_stack<>;
template class foonathan::memory::memory_stack_raii_unwind<memory_stack<>>;
template class foonathan::memory::allocator_traits<memory_stack<>>;
template class foonathan::memory::composable_allocator_traits<memory_stack<>>;
#endif
