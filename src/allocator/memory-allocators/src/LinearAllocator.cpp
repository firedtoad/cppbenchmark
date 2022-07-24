#include "LinearAllocator.h"
#include "Utils.h"   /* CalculatePadding */
#include <algorithm> // max
#include <cassert>   /*assert		*/
#include <stdlib.h>  /* malloc, free */
#ifdef _DEBUG
#include <iostream>
#endif

LinearAllocator::LinearAllocator(const std::size_t totalSize) : Allocator(totalSize) {}

void LinearAllocator::Init()
{
    if (m_start_ptr != nullptr)
    {
        free(m_start_ptr);
    }
    m_start_ptr = malloc(m_totalSize);
    m_offset    = 0;
}

LinearAllocator::~LinearAllocator()
{
    free(m_start_ptr);
    m_start_ptr = nullptr;
}

void *LinearAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
    std::size_t padding              = 0;
    std::size_t paddedAddress        = 0;
    const std::size_t currentAddress = (std::size_t)m_start_ptr + m_offset;

    if (alignment != 0 && m_offset % alignment != 0)
    {
        // Alignment is required. Find the next aligned memory address and update offset
        padding = Utils::CalculatePadding(currentAddress, alignment);
    }

    if (m_offset + padding + size > m_totalSize)
    {
        return nullptr;
    }

    m_offset += padding;
    const std::size_t nextAddress = currentAddress + padding;
    m_offset += size;

#ifdef _DEBUG
    std::cout << "A"
              << "\t@C " << (void *)currentAddress << "\t@R " << (void *)nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
#endif

    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return (void *)nextAddress;
}

void LinearAllocator::Free(void *ptr)
{
    assert(false && "Use Reset() method");
}

void LinearAllocator::Reset()
{
    m_offset = 0;
    m_used   = 0;
    m_peak   = 0;
}
