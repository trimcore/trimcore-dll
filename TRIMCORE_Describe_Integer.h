#ifndef TRIMCORE_DLL_DESCRIBE_INTEGER_H
#define TRIMCORE_DLL_DESCRIBE_INTEGER_H

#include <string_view>
#include <vector>
#include <charconv>

namespace TRIMCORE {

    // Describe (integer)
    //  - parameters
    //     - "base" or directly the number as first parameter is numeric base
    //        - example: Describe (123, L"16"); or Describe (123, L"base<16>"); or hexadecimal Describe (123, L"X");
    //     - "rsrc" to use the number and load stringtable resource string from calling module, optionally with offset
    //        - example: Describe (1, L"rsrc<+0x1000>") - loads string 0x1001
    //  - after conversion, the string is passed to Describe (string) for case change, padding, trimming, etc.

    inline std::wstring Describe (signed char v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (signed short v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (signed int v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (signed long v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (signed long long v, DescriptionFormatting * parameter = nullptr);

    inline std::wstring Describe (unsigned char v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (unsigned short v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (unsigned int v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (unsigned long v, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (unsigned long long v, DescriptionFormatting * parameter = nullptr);

    // DescriptionLengthEst
    //  - estimation based on base 10 representation

    inline std::size_t DescriptionLengthEst (std::int8_t) { return 3; }
    inline std::size_t DescriptionLengthEst (std::uint8_t) { return 3; }
    inline std::size_t DescriptionLengthEst (std::int16_t) { return 5; }
    inline std::size_t DescriptionLengthEst (std::uint16_t) { return 5; }
    inline std::size_t DescriptionLengthEst (std::int32_t) { return 10; }
    inline std::size_t DescriptionLengthEst (std::uint32_t) { return 10; }
    inline std::size_t DescriptionLengthEst (signed long) { return 10; }
    inline std::size_t DescriptionLengthEst (unsigned long) { return 10; }
    inline std::size_t DescriptionLengthEst (std::int64_t) { return 20; }
    inline std::size_t DescriptionLengthEst (std::uint64_t) { return 20; }
}

#include "TRIMCORE_Describe_Integer.tcc"
#endif

