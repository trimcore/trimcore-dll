#ifndef TRIMCORE_DLL_DESCRIBE_STRING_TCC
#define TRIMCORE_DLL_DESCRIBE_STRING_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t DescWcs (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const wchar_t *, std::size_t) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t DescWcsLen (DescriptionFormatting * fmt, const wchar_t * buffer, std::size_t length) noexcept;
}

inline std::wstring TRIMCORE::Describe (std::wstring_view string, DescriptionFormatting * format) {
    if (auto n = Implementation::DescWcsLen (format, string.data (), string.length ())) {

        std::wstring result;
        result.resize (n);

        if ((n = Implementation::DescWcs (&result [0], n, format, string.data (), string.length ()))) {

            result.resize (n);
            return result;
        } else
            return std::wstring ();
    } else
        return std::wstring (string);
}

#endif
