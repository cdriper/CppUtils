
#pragma once 

#include "Common/NonCopyable.h"

namespace Platform
{
    struct HeapState 
    {
        bool    m_available = false; 
        int64_t m_allocatedBlocks = 0;
        int64_t m_allocatedBytes  = 0;

        operator bool () const 
        {
            return m_available;
        }

        std::string ToString() const 
        {
            if (!m_available)
                return "N/A";

            return "bytes " + std::to_string(m_allocatedBytes) + 
                "; blocks " + std::to_string(m_allocatedBlocks);
        }
    };

    // ------------------------------------------------------------

    HeapState GetHeapState();

    // ------------------------------------------------------------

    class HeapTracker : Common::NonCopyable
    {
        HeapState m_state;

    public:

        HeapTracker()
        {
            Reset();
        }

        void Reset()
        {
            m_state = GetHeapState();
        }

        HeapState Get() const 
        {
            auto curr = GetHeapState();

            if (!curr.m_available || !m_state.m_available)
                return {};

            curr.m_allocatedBlocks -= m_state.m_allocatedBlocks;
            curr.m_allocatedBytes  -= m_state.m_allocatedBytes;

            return curr; 
        }

    };


}

