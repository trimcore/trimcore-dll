#ifndef TRIMCORE_DLL_DESCRIBE_STRING_H
#define TRIMCORE_DLL_DESCRIBE_STRING_H

#if (defined(__GNUC__) && (__GNUC__ < 8))
#include <experimental/string_view>
#else
#include <string_view>
#endif

#include <string>

namespace TRIMCORE {

    // Describe (string)
    //  - mostly set of forwarders from all different kinds of string to std::wstring
    //  - NULL pointer is converted to <null> with singalling 'invalid' flag, see below
    //  - supports following conversions:
    //     - "uppercase" ("upper" or "A") converts to uppercase, locale ("L" parameter) sensitive
    //     - "lowercase" ("lower" or "a") converts to lowercase, locale ("L" parameter) sensitive
    //     - "titlecase" converts to title casing, locale ("L" parameter) sensitive
    //     - "trim" or "rtrim"/"Rtrim" and "ltrim"/"Ltrim" to remove leading/trailing whitespace
    //     - "pad<N>" adds extra padding spaces so that whole string is N characters long
    //         - "lpad"/"Lpad" or "rpad"/"Rpad" to pad from only one side
    //         - "padding<abc>" will use "abc" to pad, instead of spaces; centering
    //     - "max<N>" - limits string length to N characters
    //         - additional "..." parameter will terminate excessive long string with "..."
    //            - additional "U" parameter will use single Unicode horizontal ellipsis character instead
    //     - "normalize<C|D|KC|KD>" - performs unicode normalization (folding)
    //         - also spelled "fold<X>" - where X defaults to 'C'
    //         - "unfold" is alias to "normalize<KD>"
    //     - "strip" - removes diacritics and accent marks from the string

    inline std::wstring Describe (std::wstring string, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (std::wstring_view string, DescriptionFormatting * format = nullptr);

    inline auto Describe (char c, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::a2w (&c, 1), format); }
    inline auto Describe (char8_t c, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::u2w (&c, 1), format); }
    inline auto Describe (wchar_t c, DescriptionFormatting * format = nullptr) { return Describe (std::wstring_view (&c, 1), format); }
    inline auto Describe (const std::string & s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::a2w (s), format); }
    inline auto Describe (const std::u8string & s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::u2w (s), format); }
    inline auto Describe (std::string_view s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::a2w (s), format); }
    inline auto Describe (std::u8string_view s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::u2w (s), format); }

    inline std::wstring DescribeNullString (DescriptionFormatting * format) {
        if (format) {
            format->quoted = false;
            format->invalid = true;
        }
        return L"<null>";
    }

    inline auto Describe (const char * s, DescriptionFormatting * format = nullptr) {
        if (s) {
            return Describe (TRIMCORE::a2w (s), format);
        } else
            return DescribeNullString (format);
    }

    inline auto Describe (const char8_t * s, DescriptionFormatting * format = nullptr) {
        if (s) {
            return Describe (TRIMCORE::u2w (s), format);
        } else
            return DescribeNullString (format);
    }

    inline auto Describe (const wchar_t * string, DescriptionFormatting * format = nullptr) {
        if (string) {
            return Describe (std::wstring_view (string), format);
        } else
            return DescribeNullString (format);
    }

    inline auto Describe (char * string, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const char *> (string), format);
    }
    inline auto Describe (char8_t * string, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const char8_t *> (string), format);
    }
    inline auto Describe (wchar_t * string, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const wchar_t *> (string), format);
    }
}

#include "TRIMCORE_Describe_String.tcc"
#endif

