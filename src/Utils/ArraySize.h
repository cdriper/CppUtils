#pragma once 

namespace Utils
{
    template<typename T, size_t size> 
    size_t ArraySize(T (&)[size]) { return size; };     
}