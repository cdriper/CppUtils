
#include "stdafx.h"

#include "Utils/SyncMap.h"

namespace 
{
    struct OS_SubscrHandle { /* ... */ };
    struct OS_Event { /* ... */ }; 
    struct OS_EventFilter { /* ... */ }; 

    using SubscrCallback = void (OS_Event& event, void* userData);

    OS_SubscrHandle OS_EventsSubscribe(const OS_EventFilter& filter, void* userData, SubscrCallback callback)
    {
        return {};
    }

    void OS_EventsUnsubscribe(OS_SubscrHandle handle)
    {
    }

    // -----------------------------------------------------------

    using EvCallback = std::function<void (const OS_Event&)>;

    class WrapperV1 : Common::NonCopyable {
        OS_SubscrHandle m_handle; 
        EvCallback      m_callback;

        static void RawCallback(OS_Event& event, void* userData) {
            auto p = (WrapperV1*)userData;

            p->m_callback(event);
        }

    public:

        WrapperV1(const OS_EventFilter& filter, const EvCallback& callback) : 
            m_handle( OS_EventsSubscribe(filter, this, &WrapperV1::RawCallback) ),
            m_callback(callback) {
        }

        ~WrapperV1() {
            OS_EventsUnsubscribe(m_handle);
            m_callback = {};
        }

    };

}

// -------------------------------------------------------------

namespace
{
    using namespace Utils; 

    void SyncMapTestImpl()
    {
        struct X
        {
            int m_val;

            X(int val) : m_val(val)
            {
            }

            bool Equal(int val)
            {
                return val == m_val;
            }
        };

        SyncMap<X*> sm{}; 

        void* key1 = 0;

        // first key/value
        {
            X x1{1};

            SyncMap<X*>::Key k{sm, &x1};

            key1 = k.GetKey();

            {
                SyncMap<X*>::ValueLocker v{sm, key1};
                CMN_ASSERT( v );
                CMN_ASSERT( (*v)->Equal(1) );
            }
        }

        // second key/value
        {
            X x2{2};

            SyncMap<X*>::Key k{sm, &x2};

            auto key2 = k.GetKey();

            CMN_ASSERT(key1 != key2);

            // first key is unregistered 
            {
                SyncMap<X*>::ValueLocker v{sm, key1};
                CMN_ASSERT( !v );
            }

            {
                SyncMap<X*>::ValueLocker v{sm, key2};
                CMN_ASSERT( v );
                CMN_ASSERT( (*v)->Equal(2) );
            }
        }

    }

}

// -------------------------------------------------------------

namespace UtilsTests
{
    using namespace Utils; 

    void SyncMapTest()
    {
        SyncMapTestImpl();
    }

}
