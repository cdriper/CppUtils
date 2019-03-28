#pragma once 

namespace Utils
{

    inline std::string PrefixFormat(uint64_t number, int div = 1024, int precision = 1,
        const std::string& format = "{}{}")
    {
        const std::string c_pref = "KMGTP";

        if (number < div) 
            return fmt::format( format, number, "" );

        int i = 0;
        uint64_t k = div;

        while(true)
        {
            if (i == c_pref.length() - 1)  // max
                return "<out of range>";

            if (number / k < div) 
                break;

            // next
            ++i;
            k *= div;
        }

        double val = ((double)number) / k;

        auto num = fmt::format("{:.{}f}", val, precision);

        return fmt::format( format, num, c_pref.at(i) ); 
    }


    
}  // namespace Utils

