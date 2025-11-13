#ifndef TRIMCORE_DLL_DESCRIBE_STRING_TCC
#define TRIMCORE_DLL_DESCRIBE_STRING_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescWcs (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const wchar_t *, std::size_t) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescWcsLen (DescriptionFormatting * fmt, const wchar_t * buffer, std::size_t length) noexcept;

    inline std::wstring DescribeCallDescWcs (std::size_t n, DescriptionFormatting * format, const wchar_t * data, std::size_t size) {
        std::wstring result;
        result.resize (n);

        if ((n = Implementation::DescWcs (&result [0], n, format, data, size))) {

            result.resize (n);
            return result;
        } else
            return std::wstring ();
    }
}

inline std::wstring TRIMCORE::Describe (std::wstring_view string, DescriptionFormatting * format) {
    if (format && format->length) {
        switch (auto n = Implementation::DescWcsLen (format, string.data (), string.length ())) {
            case ~0:
                return std::wstring (string);
            case 0:
                return std::wstring ();
            default:
                return Implementation::DescribeCallDescWcs (n, format, string.data (), string.length ());
        }
    } else
        return std::wstring (string);
}

inline std::wstring TRIMCORE::Describe (std::wstring string, DescriptionFormatting * format) {
    if (format && format->length) {
        switch (auto n = Implementation::DescWcsLen (format, string.data (), string.length ())) {
            case ~0:
                return string;
            case 0:
                return std::wstring ();
            default:
                return Implementation::DescribeCallDescWcs (n, format, string.data (), string.length ());
        }
    } else
        return string;
}

#endif
