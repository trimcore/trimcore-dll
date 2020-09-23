#ifndef TRIMCORE_DLL_DESCRIBE_STRING_H
#define TRIMCORE_DLL_DESCRIBE_STRING_H

#include <string_view>
#include <string>

namespace TRIMCORE {

    // Describe (string)
    //  - mostly set of forwarders from all different kinds of string to std::wstring
    //  - NULL pointer is converted to <null> with singalling 'invalid' flag, see below
    //  - supports following conversions:
    //     - <nothing is implemented yet>

    inline std::wstring Describe (std::wstring_view string, DescriptionFormatting * format = nullptr);

    inline std::wstring Describe (const std::wstring & string, DescriptionFormatting * format = nullptr) {
        return Describe (std::wstring_view (string), format);
    }

    inline std::wstring Describe (char c, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::a2w (&c, 1), format); }
    inline std::wstring Describe (char8_t c, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::u2w (&c, 1), format); }
    inline std::wstring Describe (wchar_t c, DescriptionFormatting * format = nullptr) { return Describe (std::wstring_view (&c, 1), format); }
    inline std::wstring Describe (const std::string & s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::a2w (s), format); }
    inline std::wstring Describe (const std::u8string & s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::u2w (s), format); }
    inline std::wstring Describe (std::string_view s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::a2w (s), format); }
    inline std::wstring Describe (std::u8string_view s, DescriptionFormatting * format = nullptr) { return Describe (TRIMCORE::u2w (s), format); }

    inline std::wstring DescribeNullString (DescriptionFormatting * format) {
        if (format) {
            format->quoted = false;
            format->invalid = true;
        }
        return L"<null>";
    }

    inline std::wstring Describe (const char * s, DescriptionFormatting * format = nullptr) {
        if (s) {
            return Describe (TRIMCORE::a2w (s), format);
        } else
            return DescribeNullString (format);
    }

    inline std::wstring Describe (const char8_t * s, DescriptionFormatting * format = nullptr) {
        if (s) {
            return Describe (TRIMCORE::u2w (s), format);
        } else
            return DescribeNullString (format);
    }

    inline std::wstring Describe (const wchar_t * string, DescriptionFormatting * format = nullptr) {
        if (string) {
            return Describe (std::wstring_view (string), format);
        } else
            return DescribeNullString (format);
    }

    inline std::wstring Describe (char * string, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const char *> (string), format);
    }
    inline std::wstring Describe (char8_t * string, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const char8_t *> (string), format);
    }
    inline std::wstring Describe (wchar_t * string, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const wchar_t *> (string), format);
    }

    // DescriptionLengthEst
    //  - estimate resulting length to be equal to source (might be wrong if there's padding/trimming requested)

    inline std::size_t DescriptionLengthEst (char) { return 1; }
    inline std::size_t DescriptionLengthEst (char8_t) { return 1; }
    inline std::size_t DescriptionLengthEst (wchar_t) { return 1; }

    inline std::size_t DescriptionLengthEst (char * s) { return s ? std::basic_string_view <char> (s).length () : 3; }
    inline std::size_t DescriptionLengthEst (char8_t * s) { return s ? std::basic_string_view <char8_t> (s).length () : 3; }
    inline std::size_t DescriptionLengthEst (wchar_t * s) { return s ? std::basic_string_view <wchar_t> (s).length () : 3; }

    inline std::size_t DescriptionLengthEst (const char * s) { return s ? std::basic_string_view <char> (s).length () : 3; }
    inline std::size_t DescriptionLengthEst (const char8_t * s) { return s ? std::basic_string_view <char8_t> (s).length () : 3; }
    inline std::size_t DescriptionLengthEst (const wchar_t * s) { return s ? std::basic_string_view <wchar_t> (s).length () : 3; }

    inline std::size_t DescriptionLengthEst (const std::string & s) { return s.length (); }
    inline std::size_t DescriptionLengthEst (const std::wstring & s) { return s.length (); }
    inline std::size_t DescriptionLengthEst (const std::u8string & s) { return s.length (); }

    inline std::size_t DescriptionLengthEst (std::string_view s) { return s.length (); }
    inline std::size_t DescriptionLengthEst (std::u8string_view s) { return s.length (); }
    inline std::size_t DescriptionLengthEst (std::wstring_view s) { return s.length (); }
}

#include "TRIMCORE_Describe_String.tcc"
#endif

