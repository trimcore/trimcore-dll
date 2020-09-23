#ifndef TRIMCORE_DLL_DESCRIBE_FLOAT_H
#define TRIMCORE_DLL_DESCRIBE_FLOAT_H

namespace TRIMCORE {

    // Describe (float)
    //  - uses either 'std::to_chars' or ...
    //  - after conversion, the string is passed to Describe (string) for case change, padding, trimming, etc.

    template <typename T>
    inline std::wstring DescribeFundamentalFloat (T number, DescriptionFormatting * parameter);

    inline std::wstring Describe (float number, DescriptionFormatting * parameter = nullptr) { return DescribeFundamentalFloat (number, parameter); }
    inline std::wstring Describe (double number, DescriptionFormatting * parameter = nullptr) { return DescribeFundamentalFloat (number, parameter); }
    inline std::wstring Describe (long double number, DescriptionFormatting * parameter = nullptr) { return DescribeFundamentalFloat (number, parameter); }
}

#include "TRIMCORE_Describe_Float.tcc"
#endif

