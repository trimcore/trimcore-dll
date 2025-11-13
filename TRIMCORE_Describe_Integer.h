#ifndef TRIMCORE_DLL_DESCRIBE_INTEGER_H
#define TRIMCORE_DLL_DESCRIBE_INTEGER_H

#if (defined(__GNUC__) && (__GNUC__ < 8))
#include <experimental/string_view>
#else
#include <string_view>
#endif

#include <vector>
#include "double_integer.hpp"

namespace TRIMCORE {

    // Describe (integer)
    //  - parameters
    //     - "base" or directly the number as first parameter is numeric base
    //        - example: Describe (123, L"16"); or Describe (123, L"base<16>"); or hexadecimal Describe (123, L"X");
    //        - zero-prefixes are supported with "X" parameter, e.g: "X:8" will prefix '0' to yield at least 8 characters
    //        - presence of "k" parameter chooses hexadecimal/decimal representation proper for error code
    //     - "rsrc" to use the number and load stringtable resource string from calling module, optionally with offset
    //        - example: Describe (1, L"rsrc<+0x1000>") - loads string 0x1001
    //     - U - upgrades '-' symbol to unicode one, which is the same width as + (alternatively write "unicode")
    //     - + - displays '+' for positive values; use +! to display + for 0
    //     - ? - will cause value of 0 to NOT print, and quotes completely removed (alternatively write "opt" or "optional")
    //     - L - presence of locale requests localization API to render the number
    //         - additional "$" or "currency" requests localization API to render the number currency format
    //         - presence of "duration" will use localization API to render duration numeric format (number is seconds)
    //         - alternatively write "locale"
    //     - "cb" - displays dynamic size information, e.g. 123456789u -> "117.7 MB"
    //            - with L uses local number format, e.g. "117,74 MB" for "cs-CZ"
    //               - word "bytes" is used instead of "B"
    //     - "cbs" - like "cb" but uses standard SI units, KiB instead of kB, MiB instead of MB, ...
    //     - "cbd" - like "cb" but divides by 1000 instead of 1024, e.g. 123456789u -> "123.5 MB"
    //     - "cbc" - like "cb" but omits trailing "B" or "bytes" string
    //     - "cd" - like "cbd" but omits trailing "B" or "bytes" string
    //  - after conversion, the string is internally passed to Describe (string) for case change, padding, trimming, etc.

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

    template <typename LO, typename HI>
    inline std::wstring Describe (const double_integer <LO, HI> & v, DescriptionFormatting * parameter = nullptr);
}

#include "TRIMCORE_Describe_Integer.tcc"
#endif

