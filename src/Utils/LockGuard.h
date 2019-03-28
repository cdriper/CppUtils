#pragma once 

#include "Common/NonCopyable.h"
#include "Common/ErrorHandling.h"

#include "LockGuardOp.h"

namespace Utils
{

    template<class T>
    void operator << (T& m, LockGuardOp op)
    {
        if (op == LockGuardOp::Lock)
            m.lock();
        else
            m.unlock();
    }

    // ------------------------------------------------------------

    /*
        RAII for C++ concept BasicLockable (.lock() and .unlock())
        it's like std::lock_guard<>, but
        #1. you can use any class by providing special << operator for LockGuardOp 
        #2. it's movable, so you can deduce type with CreateLockGuard() function 

    */

    class LockGuard : Common::NonCopyable
    {
        std::function<void ()> m_unlock; 

    public:

        template<class T>
        LockGuard(T& lock) 
        {
            lock << LockGuardOp::Lock;

            // unlock functor 
            T* pLock = &lock;

            m_unlock = [pLock]()
            {
                (*pLock) << LockGuardOp::Unlock;
            };
        }

        LockGuard(LockGuard&& other)
        {
            CMN_ASSERT(other.m_unlock);

            std::swap(m_unlock, other.m_unlock);
        }

        ~LockGuard()
        {
            if (!m_unlock)
                return;

            m_unlock();
        }

    };

    // ------------------------------------------------------------

    template<class T>
    LockGuard CreateLockGuard(T& lock)
    {
        return LockGuard{lock};
    }

}  // namespace Utils

