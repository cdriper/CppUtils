
#include <crtdbg.h>

#include "Platform/HeapState.h"

#include "Utils/Algorithm.h"

namespace Platform
{
    HeapState GetHeapState()
    {
        HeapState res{};

        _CrtMemState state;
        Utils::BinaryFill(state);

        _CrtMemCheckpoint(&state);

        for(auto v : state.lCounts)
        {
            res.m_allocatedBlocks += v;
        }

        for(auto v : state.lSizes)
        {
            res.m_allocatedBytes += v;
        }

        res.m_available = true; 

        return res;
    }

}