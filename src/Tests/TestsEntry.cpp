
#include "stdafx.h"

namespace UtilsTests
{
    void LockGuardTests();
    void SyncMapTest();
}

namespace Tests
{
    void RunTests()
    {
        UtilsTests::LockGuardTests();
        UtilsTests::SyncMapTest();

        std::cout << "Testing is completed!" << std::endl; 
    }

}


