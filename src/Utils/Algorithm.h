
#pragma once 

namespace Utils
{
    template<class T>
    void BinaryFill(T& structure, uint8_t fillCh = 0)
    {
        std::memset(&structure, fillCh, sizeof(T));
    }

	template<class T>
	void BinarySame(const T& lhs, const T& rhs)
	{
		return std::memcmp(&lhs, &rhs, sizeof(T));
	}
    
}
