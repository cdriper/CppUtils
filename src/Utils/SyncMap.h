#pragma once 

#include "Common/NonCopyable.h"
#include "Common/ErrorHandling.h"

#include "LockGuard.h"

namespace Utils
{

    /*
        ** Description **

        Thread safe wrapper around std::map. 
        You define key/value lifetime by instancing Key object. 
        You can "use" value only by holding mutex (locking whole SyncMap object). 
        See unit test as usage example.
        T must be a copyable type (it's possible to add support for movable type).

        ** Key management **
        - don't use raw pointer as a key because it's possible to get same address from heap (key collision)
        - key is 32 bits, use highest byte as a "magic" signature 

        ** Best solution for instantiation **

        // Lazy initialization, object is never destroyed

        auto Instance()
        {
            static g_instance = new SyncMap<T>{};

            return g_instance;
        }

    */
    template<class T>
    class SyncMap : Common::NonCopyable
    {

    public:

        class Key : Common::NonCopyable
        {
            SyncMap<T>& m_parent; 
            T           m_value;
            void*       m_key;

        public:
            Key(SyncMap<T>& parent, const T& value) : m_parent(parent), m_value(value)
            {
                m_key = m_parent.AddValue(&m_value);
            }

            ~Key()
            {
                m_parent.RemoveKeyValue(m_key);
            }

            void* GetKey() const 
            {
                return m_key;
            }
        };

    public:

        class ValueLocker : Common::NonCopyable
        {
            SyncMap<T>&     m_parent;
            T*              m_pValue; 

        public:

            ValueLocker(SyncMap<T>& parent, void* key) : m_parent(parent)
            {
                m_parent.m_mutex.lock();

                m_pValue = m_parent.GetValue(key);
            }

            ~ValueLocker()
            {
                m_parent.m_mutex.unlock();
            }

        public:

            bool Empty() const 
            {
                return !m_pValue;
            }

  		    operator bool () const
		    {
                return !Empty();
		    }

            T& operator*() const
            {
                CMN_ASSERT( !Empty() );

                return *m_pValue;
            }

            T* operator->() const
		    {
                CMN_ASSERT( !Empty() );

			    return m_pValue;
		    }

        };

    private:

        enum
        {
            c_magicByte = 0xa3,
        };

        void* AllocNewKey()
        {
            static_assert( sizeof(void*) >= sizeof(m_currKey), "" );

            auto next = [&]() -> void*
            {
                m_currKey += 1;

                auto v = (m_currKey & 0xffffff) | (c_magicByte << 24);

                return (void*)v;
            };

            while (true)
            {
                auto key = next();

                if ( m_map.find(key) == m_map.end() )
                    return key;
            }
        }

        bool KeyIsValid(void* key) const 
        {
            auto v64 = (uint64_t)key;  // cast thru 64 bits to avoid clang error

            uint32_t v = v64;

            return ((v >> 24) & 0xff) == c_magicByte;
        }

    // Key API
    private:

        void* AddValue(T* val)
        {
            auto _ = CreateLockGuard(m_mutex);

            auto key = AllocNewKey();

            m_map[key] = val;

            return key;
        }

        void RemoveKeyValue(void* key)
        {
            auto _ = CreateLockGuard(m_mutex);

            CMN_ASSERT( KeyIsValid(key) );

            auto i = m_map.find(key);
            CMN_ASSERT( i != m_map.end() );

            m_map.erase(i);
        }

    // ValueLocker API (w/o lock, lock is outside)
    private:

        T* GetValue(void* key) const 
        {
            CMN_ASSERT( KeyIsValid(key) );

            auto i = m_map.find(key);

            return ( i != m_map.end() ) ? i->second : 0;
        }

    public:

        SyncMap()
        {
        }

        ~SyncMap()
        {
            auto _ = CreateLockGuard(m_mutex);

            CMN_ASSERT( m_map.empty() );
        }

    private:

        std::mutex                  m_mutex; 
        std::map<void*, T*>         m_map;
        std::uint32_t               m_currKey = 0; 
    };

    
}  // namespace Utils



