#ifndef TRIMCORE_DLL_DESCRIBE_FLOAT_H
#define TRIMCORE_DLL_DESCRIBE_FLOAT_H

namespace TRIMCORE {

    // Describe (float)
    //  - parameters
    //     - "base" is numeric base
    //         - only 10 and 16 are supported
    //         - example: Describe (123.45, L"16"); or Describe (123.45, L"base<16>"); or hexadecimal Describe (123.45, L"X");
    //         - zero-prefixes are supported with "X" parameter, e.g: "X:8" will prefix '0' to yield at least 8 characters
    //     - "precision" (or "digits" or "decimals")
    //         - number of decimal places; overriden by "L"
    //     - "rsrc" to use the number and load stringtable resource string from calling module, optionally with offset
    //         - example: Describe (1.23, L"rsrc<+0x1000>") - loads string 0x1001
    //     - U - upgrades certain symbols to unicode ones (alternatively write "unicode")
    //         - '-' (minus) to proper minus, which is the same width as +
    //         - renders infinity with a proper symbol
    //     - + - displays '+' for positive values; use +! to display + for 0
    //     - ? - will cause value of 0 to NOT print, and quotes completely removed (alternatively write "opt" or "optional")
    //     - L - presence of locale requests localization API to render the number
    //         - additional "$" or "currency" requests localization API to render the number currency format
    //         - presence of "duration" will use localization API to render duration numeric format (number is in seconds)
    //         - alternatively write "locale"
    //     - Q - uses adaptive rounding method to provide nice number
    //            - use "Q:3" (where 3 is 1..15) to round at place where 3 subsequent '0' or '9' are found (default is 4)
    //         - add Qmax and/or Qmin to specify minimum and/or maximum number of decimal numbers (defaults 0...unlimited)
    //         - no other formatting, conversion or precision must not be used for this to apply
    //         - alternatively write "arounded"
    //     - "cb" - displays dynamic size information, e.g. 123456789u -> "117.7 MB"
    //            - with L uses local number format, e.g. "117,74 MB" for "cs-CZ"
    //               - word "bytes" is used instead of "B"
    //     - "cbs" - like "cb" but uses standard SI units, KiB instead of kB, MiB instead of MB, ...
    //     - "cbd" - like "cb" but divides by 1000 instead of 1024, e.g. 123456789u -> "123.5 MB"
    //     - "cd" - like "cbd" but omits trailing "B" or "bytes" string
    //  - after conversion, the string is internally passed to Describe (string) for case change, padding, trimming, etc.

    inline std::wstring Describe (float number, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (double number, DescriptionFormatting * parameter = nullptr);
    inline std::wstring Describe (long double number, DescriptionFormatting * parameter = nullptr);
}

#include "TRIMCORE_Describe_Float.tcc"
#endif
