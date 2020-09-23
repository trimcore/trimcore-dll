#ifndef TRIMCORE_DLL_DESCRIBE_PTR_TCC
#define TRIMCORE_DLL_DESCRIBE_PTR_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t DescPtr (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const void *) noexcept;
}

inline std::wstring TRIMCORE::Describe (const void * ptr, DescriptionFormatting * format) {
    if (format && format->length) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescPtr (buffer.data (), buffer.size (), format, ptr);
        return Describe (std::wstring_view (buffer.data (), length), format);
    } else {
        wchar_t buffer [12];
        auto length = Implementation::DescPtr (buffer, sizeof buffer / sizeof buffer [0], format, ptr);
        return Describe (std::wstring_view (buffer, length), format);
    }
}

#endif
