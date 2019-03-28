
#include "stdafx.h"

#include "Common/ErrorHandling.h"

#include "Utils/LockGuard.h"


// ------------------------------------------------------------

namespace Tests
{
    void RunTests();
}

// ------------------------------------------------------------

int main(int argc, char **argv) 
{
    try
    {
        Tests::RunTests();
    }
    catch(const std::exception& e)
    {
        auto msg = e.what();
        Common::ExitWithMessage(msg);
    }
}

