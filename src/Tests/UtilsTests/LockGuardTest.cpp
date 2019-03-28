
#include "stdafx.h"

#include "Platform/HeapState.h"

#include "Utils/LockGuard.h"

namespace
{
    void StdMutexTest()
    {
        std::mutex m{};

        {
            auto _ = Utils::CreateLockGuard(m);

            CMN_ASSERT( !m.try_lock() );
        }

        CMN_ASSERT( m.try_lock() );

        m.unlock();
    }
}


// --------------------------------------------------

namespace
{
    /*
    template<class Mutex>
    std::lock_guard<Mutex> make_lock_guard(Mutex& mutex) 
    {
        mutex.lock();
        return { mutex, std::adopt_lock };
    }

    void StdMutexStdGuardTest()
    {
        std::mutex m{};

        {
            auto _ = make_lock_guard(m);

            CMN_ASSERT( !m.try_lock() );
        }

        CMN_ASSERT( m.try_lock() );

        m.unlock();
    }
    */

}

// --------------------------------------------------

namespace
{
    class Mutex
    {
        bool m_locked = false; 

    public:

        ~Mutex()
        {
            CMN_ASSERT(!m_locked);
        }

        void Lock()
        {
            CMN_ASSERT(!m_locked);

            m_locked = true; 
        }

        void Unlock()
        {
            CMN_ASSERT(m_locked);

            m_locked = false; 
        }

        bool IsLocked() const 
        {
            return m_locked;
        }

    };

    // --------------------------------------------------

    void operator << (Mutex& m, Utils::LockGuardOp op)
    {
        if (op == Utils::LockGuardOp::Lock)
            m.Lock();
        else
            m.Unlock();
    }

    // --------------------------------------------------


    void CustomTypeTest()
    {
        Mutex m{};

        {
            auto _ = Utils::CreateLockGuard(m);

            CMN_ASSERT( m.IsLocked() );
        }

        CMN_ASSERT( !m.IsLocked() );
    }
}

// --------------------------------------------------

namespace
{
    void Print(const std::string& s)
    {
        std::cout << s << std::endl; 
    }

    void AllocTest()
    {
        {
            Platform::HeapTracker t{};

            auto buff = std::make_unique<uint8_t[]>(1024);

            Print( t.Get().ToString() );
        }

        {
            Platform::HeapTracker t{};

            Mutex m{};

            auto _ = Utils::CreateLockGuard(m);

            Print( t.Get().ToString() );
        }

        // function 
        {
            Platform::HeapTracker t{};

            // Windows, x64
            // sizeof(f) = 64
            // store up to 48 bytes of lambda's capture 
            std::array<char, 64 - 16> buff;

            std::function<void ()> f0 = [buff](){ return buff[0]; };

            Print( 
                fmt::format("sizeof f {}; size of buff {}; heap: {}", sizeof(f0), sizeof(buff), t.Get().ToString() ) 
            );
        }
    }
}

// --------------------------------------------------

namespace UtilsTests
{

    void LockGuardTests()
    {
        StdMutexTest();
        CustomTypeTest();
        // AllocTest();

    }

}
