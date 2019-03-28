#pragma once 

#include "ErrorHandling.h"
#include "NonCopyable.h"

namespace Common
{
    
    class FuncRAII : NonCopyable
    {
        std::function<void ()> m_fn;

    public:
        
        FuncRAII(const std::function<void ()>& fn) : m_fn(fn)
        {
            CMN_ASSERT( m_fn );
        }
        
        ~FuncRAII()
        {
            m_fn();
        }

    };    
    
}